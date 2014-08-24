#include "CCodeGenTestsBase.h"

class CCodeGenTests_Closures : public CCodeGenTestsBase {
};
/*
TEST_F(CCodeGenTests_Closures, ClosureVariableCapture) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  Int x = 0\n"
                                                  "  Int y = 0\n"
                                                  "  {Int} closure = do (Int arg1) {\n"
                                                  "    x + y\n"
                                                  "  }\n"
                                                  "end\n");

    EXPECT_EQ("// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "    const int x;\n"
              "    const int y;\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env, int arg1) {\n"
              "    (self_env->x) + (self_env->y);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = 0;\n"
              "    int y = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, x, y);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, ClosureVariableCaptureWithReference) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  Int x = 0\n"
                                                  "  {Int} closure = do (Int arg1; Void; x) {\n"
                                                  "    x = 5\n"
                                                  "  }\n"
                                                  "end\n");

    EXPECT_EQ("// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "    int* x;\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env, int arg1) {\n"
              "    *(self_env->x) = 5;\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = 0;\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env, &x);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_HAS_REFERENCES);\n"
              "}\n\n", visitor->bodyString());
}*/
/*
TODO: re-enable this
TEST_F(CCodeGenTests_Closures, InvokeClosurePointer) {
    ASTNode* node = this->parse("def test(*{Int value} closure)\n"
                                "    (*closure)(1)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void test(three_closure_t* closure);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(three_closure_t* closure) {\n"
              "    THREE_CALL_CLOSURE(void (*)(void*, int), (*closure), 1);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Closures, NestedClosures) {
    ASTNode* node = this->parse("def test()\n"
                                "  {} closure = do () {\n"
                                "    {} nested_closure = do () {\n"
                                "      {} double_nested_closure = do () {\n"
                                "      }\n"
                                "    }\n"
                                "  }\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("// test_closure_3\n"
              "struct test_closure_3_env {\n"
              "};\n"
              "static void test_closure_3(struct test_closure_3_env* self_env) {\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_3);\n\n"
              "// test_closure_2\n"
              "struct test_closure_2_env {\n"
              "};\n"
              "static void test_closure_2(struct test_closure_2_env* self_env) {\n"
              "    THREE_CAPTURE_ENV(test_closure_3_env);\n"
              "    three_closure_t double_nested_closure = THREE_MAKE_CLOSURE(test_closure_3, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_2);\n\n"
              "// test_closure_1\n"
              "struct test_closure_1_env {\n"
              "};\n"
              "static void test_closure_1(struct test_closure_1_env* self_env) {\n"
              "    THREE_CAPTURE_ENV(test_closure_2_env);\n"
              "    three_closure_t nested_closure = THREE_MAKE_CLOSURE(test_closure_2, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n"
              "THREE_CHECK_CLOSURE_FUNCTION(test_closure_1);\n\n", visitor->declarationsString());
    EXPECT_EQ("void test(void) {\n"
              "    THREE_CAPTURE_ENV(test_closure_1_env);\n"
              "    three_closure_t closure = THREE_MAKE_CLOSURE(test_closure_1, THREE_CLOSURE_FLAGS_DEFAULT);\n"
              "}\n\n", visitor->bodyString());
}
*/