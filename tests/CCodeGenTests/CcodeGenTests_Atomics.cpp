#include "CCodeGenTestsBase.h"

class CCodeGenTests_Atomics : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Atomics, AtomicStatement) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  atomic\n"
                                                  "    x = 1\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t test_tx_1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&test_tx_1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&test_tx_1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'test_tx_1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicStatementWithElse) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  atomic\n"
                                                  "    x = 1\n"
                                                  "  else\n"
                                                  "    x = 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/transactional_memory.h>\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t test_tx_1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&test_tx_1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&test_tx_1);\n"
              "    } else {\n"
              "        x = 0;\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicStatementWithAbort) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  atomic\n"
                                                  "    x = 1\n"
                                                  "    abort if x > 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t test_tx_1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&test_tx_1)) {\n"
              "        x = 1;\n"
              "        if (x > 0) {\n"
              "            three_transaction_abort(&test_tx_1);\n"
              "        }\n"
              "        three_transaction_end(&test_tx_1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'test_tx_1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, MemoryBarrier) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  barrier\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    atomic_thread_fence(memory_order_seq_cst);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionAdd) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  x = atomic (x += 1)\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionSubtract) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  x = atomic (x -= 1)\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_sub_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionLoad) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int x)\n"
                                                  "  Bool value = atomic (x > 0)\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(const int);\n", visitor->declarationsString());
    EXPECT_EQ("void test(const int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(const int x) {\n"
              "    const bool value = atomic_load_explicit((_Atomic(const int)*)&x, memory_order_seq_cst) > 0;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, AtomicExpressionAsStatement) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  atomic (x += 1)\n"
                                                  "end\n");

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, ReturnStatementInAtomic) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  atomic\n"
                                                  "    x = 1\n"
                                                  "    return if x > 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t test_tx_1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&test_tx_1)) {\n"
              "        x = 1;\n"
              "        if (x > 0) {\n"
              "            three_transaction_end(&test_tx_1);\n"
              "            return;\n"
              "        }\n"
              "        three_transaction_end(&test_tx_1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'test_tx_1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Atomics, ReturnBooleanInAtomic) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(; Bool)\n"
                                                  "  atomic\n"
                                                  "    return true\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("const bool test(void) {\n"
              "    three_transaction_t test_tx_1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&test_tx_1)) {\n"
              "        const bool tmp_return_value_1 = true;\n"
              "        three_transaction_end(&test_tx_1);\n"
              "        return tmp_return_value_1;\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'test_tx_1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}
