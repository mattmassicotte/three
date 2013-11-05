#include "ParserTestBase.h"
#include "compiler/CodeGen/CSourceContext.h"

class CCodeGenTests : public ParserTestBase {
};

TEST_F(CCodeGenTests, IncludeNode) {
    ASTNode* node = this->parse("include \"stdio.h\"\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <stdio.h>\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, PublicIncludeNode) {
    ASTNode* node = this->parse("public\ninclude \"stdio.h\"\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <stdio.h>\n#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionDefinition) {
    ASTNode* node = this->parse("def test()\nend\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, PrivateFunctionDefinition) {
    ASTNode* node = this->parse("private\n"
                                "def test()\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.declarations()->renderToString());
    EXPECT_EQ("", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionDefinitionWithArguments) {
    ASTNode* node = this->parse("def test(Int a, Int b; *Int)\nend\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("int* test(int a, int b);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("int* test(int a, int b) {\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, VariableDeclarationWithInitializer) {
    ASTNode* node = this->parse("def test(Int a)\nInt b = a\nend\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int a);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int a) {\n    int b = a;\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, UsingFunctionFromCHeader) {
    ASTNode* node = this->parse("include \"stdio.h\"\n\ndef test()\nprintf(\"hello world\")\nend\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n    printf(\"hello world\");\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, SwitchStatement) {
    ASTNode* node = this->parse("def test(Int x)\nswitch x\ncase 1\nx = 5\ncase 2\nx = 6\nend\nend\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n    if (x == 1) {\n        x = 5;\n    } else if (x == 2) {\n        x = 6;\n    }\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, Structure) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n    int y;\n} Foo;\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, PublicStructure) {
    ASTNode* node = this->parse("public\n"
                                "struct Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n    int y;\n} Foo;\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\ntypedef struct Foo Foo;\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, PrivateStructure) {
    ASTNode* node = this->parse("private\n"
                                "struct Foo\n"
                                "  Int x\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n} Foo;\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, PackedStructure) {
    ASTNode* node = this->parse("struct:4 Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#pragma pack(push)\n"
              "#pragma pack(4)\n"
              "typedef struct Foo {\n"
              "    int x;\n"
              "    int y;\n"
              "} Foo;\n"
              "#pragma pack(pop)\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, NamespacedEnumeration) {
    ASTNode* node = this->parse("namespace Foo\n"
                                "  enum Bar\n"
                                "    A\n"
                                "    B\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("enum {\n"
              "    Foo_3_Bar_3_A,\n"
              "    Foo_3_Bar_3_B\n"
              "};\n"
              "typedef uint32_t Foo_3_Bar;\n\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("", context.body()->renderToString());
}

TEST_F(CCodeGenTests, SimpleGlobalVariable) {
    ASTNode* node = this->parse("Int a\n");

    CSourceContext context;

    node->codeGen(context);

    // TODO: this needs to happen eventually
    // EXPECT_EQ("extern int a;\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("int a;\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionPointerVariable) {
    ASTNode* node = this->parse("*(*Void; *Void) fn_ptr\n");

    CSourceContext context;

    node->codeGen(context);

    // TODO: this needs to happen eventually
    // EXPECT_EQ("extern void* (*fn_ptr)(void* arg);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void* (*fn_ptr)(void*);\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, MemberAccess) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Natural:32 a\n"
                                "end\n"
                                "def test(Foo f)\n"
                                "  f.a = 0\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n    uint32_t a;\n} Foo;\n\nvoid test(Foo f);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(Foo f) {\n    f.a = 0;\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionCallOperator) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  *(Int) fn\n"
                                "end\n"
                                "def test(Foo f)\n"
                                "  f.fn(1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n"
              "    void (*fn)(int);\n"
              "} Foo;\n\n"
              "void test(Foo f);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(Foo f) {\n    f.fn(1);\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, StructMemberWithUnaryOperators) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Int a\n"
                                "end\n"
                                "def test(**Foo f)\n"
                                "  f = &(*f)->a\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("typedef struct Foo {\n    int a;\n} Foo;\n\nvoid test(Foo** f);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(Foo** f) {\n    f = (&(*f)->a);\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, TernaryConditionalOperator) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = x ? 0 : 1\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = x ? 0 : 1;\n"
              "}\n\n", context.body()->renderToString()); 
}

TEST_F(CCodeGenTests, InfiniteLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n    for (;;) {\n    }\n}\n\n", context.body()->renderToString()); 
}

TEST_F(CCodeGenTests, ConditionLoop) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop x > 0\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n    while (x > 0) {\n    }\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ConditionLoopWithStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop x > 0\n"
                                "    x += 1\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    while (x > 0) {\n"
              "        x += 1;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AfterConditionLoop) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop:after x > 0\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n    do {\n    } while (x > 0);\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ForLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  for (Int i = 0; i < 10; i += 1)\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; i < 10; i += 1) {\n    }\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, RangedForLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  for (Int i in 0:10)\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; (i < 10) && (0 < 10); ++i) {\n    }\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, Return) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "  return\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("int test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("int test(void) {\n    return;\n}\n\n", context.body()->renderToString());
}


TEST_F(CCodeGenTests, ReturnWithExpression) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "  return 1 + 1\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("int test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("int test(void) {\n    return 1 + 1;\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionReferences) {
    ASTNode* node = this->parse("def foo()\n"
                                "end\n"
                                "def test()\n"
                                "  *Void ptr = &foo\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void foo(void);\nvoid test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void foo(void) {\n}\n\nvoid test(void) {\n    void* ptr = (&foo);\n}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        assert(0 && \"transaction 'tx1' failed without any fallback path\");\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicStatementWithElse) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "  else\n"
                                "    x = 0\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/transactional_memory.h>\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    three_transaction_t tx1 = THREE_MAKE_DEFAULT_TRANSACTION();\n"
              "    if (three_transaction_begin(&tx1)) {\n"
              "        x = 1;\n"
              "        three_transaction_end(&tx1);\n"
              "    } else {\n"
              "        x = 0;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicStatementWithAbort) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "    abort if x > 0\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
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
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, MemoryBarrier) {
    ASTNode* node = this->parse("def test()\n"
                                "  barrier\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    atomic_thread_fence(memory_order_seq_cst);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicExpressionAdd) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = atomic (x += 1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicExpressionSubtract) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = atomic (x -= 1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = atomic_fetch_sub_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicExpressionLoad) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  Bool value = atomic (x > 0)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    bool value = atomic_load_explicit((_Atomic(int)*)&x, memory_order_seq_cst) > 0;\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, AtomicExpressionAsStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic (x += 1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <three/runtime/atomic.h>\n\n"
              "THREE_CHECK_ATOMIC(int);\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    atomic_fetch_add_explicit((_Atomic(int)*)&x, 1, memory_order_seq_cst);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ReturnStatementInAtomic) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  atomic\n"
                                "    x = 1\n"
                                "    return if x > 0\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("#include <assert.h>\n"
              "#include <three/runtime/transactional_memory.h>\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
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
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, IfStatementWithElse) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  if x > 0\n"
                                "    x = 1\n"
                                "  else\n"
                                "    x = 2\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x > 0) {\n"
              "        x = 1;\n"
              "    } else {\n"
              "        x = 2;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, IfStatementWithNonParenthesisConditional) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  if x\n"
                                "    x = 2\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x) {\n"
              "        x = 2;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, NamespacedFunction) {
    ASTNode* node = this->parse("namespace Foo\n"
                                "  def test(Int x)\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void Foo_3_test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void Foo_3_test(int x) {\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, LiteralNull) {
    ASTNode* node = this->parse("def test(*Int x)\n"
                                "  x = null\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int* x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int* x) {\n"
              "    x = NULL;\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, BooleanLiteral) {
    ASTNode* node = this->parse("def test(Bool x)\n"
                                "  x = true\n"
                                "  x = false\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(bool x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(bool x) {\n"
              "    x = true;\n"
              "    x = false;\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, Sizeof) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = sizeof(Int)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(int x);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = sizeof(int);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, BreakStatement) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "    break\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        break;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ContinueStatement) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "    continue\n"
                                "  end\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        continue;\n"
              "    }\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, FunctionWithClosureArgument) {
    ASTNode* node = this->parse("def test({} closure)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("", context.declarations()->renderToString());
    EXPECT_EQ("void test(three_closure_t closure);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(three_closure_t closure) {\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, CreateClosure) {
    ASTNode* node = this->parse("def test()\n"
                                "  Int x = 0\n"
                                "  {Int} closure = do (Int arg1) {\n"
                                "    x = 5\n"
                                "  }\n"
                                "  closure(1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("struct test_closure_1_env {\n"
              "    int x;\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env, int arg1) {\n"
              "    (self_env->x) = 5;\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, x);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "    THREE_CALL_CLOSURE(void (*)(void*, int), closure, 1);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ClosureVariableCapture) {
    ASTNode* node = this->parse("def test()\n"
                                "  Int x = 0\n"
                                "  Int y = 0\n"
                                "  {Int} closure = do (Int arg1) {\n"
                                "    x = x + y\n"
                                "  }\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("struct test_closure_1_env {\n"
              "    int x;\n"
              "    int y;\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env, int arg1) {\n"
              "    (self_env->x) = (self_env->x) + (self_env->y);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = 0;\n"
              "    int y = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, x, y);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, ClosureVariableCaptureWithReference) {
    ASTNode* node = this->parse("def test()\n"
                                "  Int x = 0\n"
                                "  {Int} closure = do (Int arg1; Void; x) {\n"
                                "    x = 5\n"
                                "  }\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("struct test_closure_1_env {\n"
              "    int* x;\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env, int arg1) {\n"
              "    *(self_env->x) = 5;\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", context.declarations()->renderToString());
    EXPECT_EQ("void test(void);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, &x);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_HAS_REFERENCES);\n"
              "}\n\n", context.body()->renderToString());
}

TEST_F(CCodeGenTests, InvokeClosurePointer) {
    ASTNode* node = this->parse("def test(*{Int value} closure)\n"
                                "    (*closure)(1)\n"
                                "end\n");

    CSourceContext context;

    node->codeGen(context);

    EXPECT_EQ("", context.declarations()->renderToString());
    EXPECT_EQ("void test(three_closure_t* closure);\n", context.internalDeclarations()->renderToString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", context.externalDeclarations()->renderToString());
    EXPECT_EQ("void test(three_closure_t* closure) {\n"
              "    THREE_CALL_CLOSURE(void (*)(void*, int), (*closure), 1);\n"
              "}\n\n", context.body()->renderToString());
}
