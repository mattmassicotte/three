#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Atomics : public ParserTestBase {
};

TEST_F(CCodeGenTests_Atomics, AtomicStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicStatementWithElse) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "  else\n"
                                "    x = 0\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/transactional_memory.h>\n\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        x = 0;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicStatementWithAbort) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "    abort if x > 0\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        if (x > 0) {\n"
              "            three_transaction_abort(&tx1);\n"
              "        }\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, MemoryBarrier) {
    ASTNode* node = this->parse("def test()\n"
                                "  barrier\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n", visitor.declarationsString());
    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    atomic_thread_fence(memory_order_seq_cst);\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionAdd) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = atomic (x += 1)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionSubtract) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = atomic (x -= 1)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_sub_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionLoad) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  Bool value = atomic (x > 0)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    bool value = atomic_load_explicit((_Atomic(int)*)&x, memory_order_seq_cst) > 0;\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionAsStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic (x += 1)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, ReturnStatementInAtomic) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "    return if x > 0\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor.declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        if (x > 0) {\n"
              "            three_transaction_end(&tx1);\n"
              "            return;\n"
              "        }\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, ReturnStatementInAtomicWithEnsure) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "    return x + 1\n"
                                "  end\n"
                                "ensure\n"
                                "  x = 5\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        int tmp_return_value_1 = x + 1;\n"
              "        three_transaction_end(&tx1);\n"
              "        // ensure clause\n"
              "        x = 5;\n"
              "        return tmp_return_value_1;\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "    // ensure clause\n"
              "    x = 5;\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Atomics, ReturnBooleanInAtomic) {
    ASTNode* node = this->parse("def test(; Bool)\n"
                                "  atomic\n"
                                "    return true\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("bool test(void) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        bool tmp_return_value_1 = true;\n"
              "        three_transaction_end(&tx1);\n"
              "        return tmp_return_value_1;\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}
