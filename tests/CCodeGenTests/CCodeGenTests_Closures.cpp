#include "CCodeGenTestsBase.h"

class CCodeGenTests_Closures : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Closures, ClosureVariableCapture) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  Int x = 0\n"
                                                  "  Int y = 0\n"
                                                  "  {Int} -> Void closure = do (Int arg1) {\n"
                                                  "    x + y\n"
                                                  "  }\n"
                                                  "end\n");

    EXPECT_EQ("// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "    const int x;\n"
              "    const int y;\n"
              "};\n"
              "static void test_closure_1(const struct test_closure_1_env* const self_env, const int arg1) {\n"
              "    (self_env->x) + (self_env->y);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    const int x = 0;\n"
              "    const int y = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, x, y);\n"
              "    const three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, ClosureVariableCaptureWithReference) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  Int x = 0\n"
                                                  "  {Int} -> Void closure = do (Int arg1; x) {\n"
                                                  "    x = 5\n"
                                                  "  }\n"
                                                  "end\n");

    EXPECT_EQ("// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "    int* const x;\n"
              "};\n"
              "static void test_closure_1(const struct test_closure_1_env* const self_env, const int arg1) {\n"
              "    *(self_env->x) = 5;\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    const int x = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, &x);\n"
              "    const three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_HAS_REFERENCES);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, InvokeClosurePointer) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(*{Int} closure)\n"
                                                  "    (*closure)(1)\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void test(const three_closure_t* const closure);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(const three_closure_t* const closure) {\n"
              "    THREE_CALL_CLOSURE(void (*)(void* const, const int), (*closure), 1);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, NestedClosures) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  {} closure = do () {\n"
                                                  "    {} nested_closure = do () {\n"
                                                  "      {} double_nested_closure = do () {\n"
                                                  "      }\n"
                                                  "    }\n"
                                                  "  }\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("// test_closure_3\n"
              "struct test_closure_3_env {\n"
              "};\n"
              "static void test_closure_3(const struct test_closure_3_env* const self_env) {\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_3);\n\n"
              "// test_closure_2\n"
              "struct test_closure_2_env {\n"
              "};\n"
              "static void test_closure_2(const struct test_closure_2_env* const self_env) {\n"
              "    THREE_CAPTURE_ENV(test_closure_3_env);\n"
              "    const three_closure_t double_nested_closure = THREE_MAKE_CLOSURE(test_closure_3, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_2);\n\n"
              "// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "};\n"
              "static void test_closure_1(const struct test_closure_1_env* const self_env) {\n"
              "    THREE_CAPTURE_ENV(test_closure_2_env);\n"
              "    const three_closure_t nested_closure = THREE_MAKE_CLOSURE(test_closure_2, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void) {\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env);\n"
              "    const three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, DISABLED_TransposedClosure) {
    Three::CCodeGenVisitor* visitor = this->visit("def foo({} c)\n"
                                                  "end\n"
                                                  "def test()\n"
                                                  "  await foo()\n"
                                                  "  Int x = 0\n"
                                                  "end\n");

    EXPECT_EQ("// test_closure_1\n"
    "struct test_closure_1_env {\n"
    "};\n"
    "static void test_closure_1(const struct test_closure_1_env* const self_env) {\n"
    "    const int x = 0;\n"
    "}\n"
    "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());

    EXPECT_EQ("void foo(three_closure_t c);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void foo(three_closure_t c) {\n"
              "}\n\n"
              "void test(void) {\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env);\n"
              "    THREE_CALL_CLOSURE(void (*)(void* const), (*closure), 1);\n"
              
              "    THREE_CALL_CLOSURE(void (*)(void* const), THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT));\n"
              "}\n\n", visitor->bodyString());
}
