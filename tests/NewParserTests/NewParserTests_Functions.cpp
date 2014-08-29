#include "../ParserTestsBase.h"

class ParserTests_Functions : public ParserTestsBase {
};

TEST_F(ParserTests_Functions, FunctionDefinition) {
    ASTNode* node = this->parseNode("def foo()\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(0, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());
}

TEST_F(ParserTests_Functions, FunctionWithOneTypedParameter) {
    ASTNode* node = this->parseNode("def foo(Int a)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, func->functionType().parameterAtIndex(0).access());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
}

TEST_F(ParserTests_Functions, FunctionWithTwoTypedParameter) {
    ASTNode* node = this->parseNode("def foo(Int a, Float b)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(2, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
    ASSERT_EQ(NewDataType::Kind::Float, func->functionType().parameterAtIndex(1).kind());
    ASSERT_EQ("b", func->functionType().parameterAtIndex(1).label());
}

TEST_F(ParserTests_Functions, FunctionWithTwoUntypedParameter) {
    ASTNode* node = this->parseNode("def foo(a, b)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(2, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Undefined, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
    ASSERT_EQ(NewDataType::Kind::Undefined, func->functionType().parameterAtIndex(1).kind());
    ASSERT_EQ("b", func->functionType().parameterAtIndex(1).label());
}

TEST_F(ParserTests_Functions, FunctionWithOneUntypedParameter) {
    ASTNode* node = this->parseNode("def foo(a)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Undefined, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
}

TEST_F(ParserTests_Functions, FunctionWithOneUntypedAndOneTypedParameter) {
    ASTNode* node = this->parseNode("def foo(a, Int b)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(2, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Undefined, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().parameterAtIndex(1).kind());
    ASSERT_EQ("b", func->functionType().parameterAtIndex(1).label());
}

TEST_F(ParserTests_Functions, FunctionWithOneTypedAndOneUnypedParameter) {
    ASTNode* node = this->parseNode("def foo(Int a, b)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(2, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
    ASSERT_EQ(NewDataType::Kind::Undefined, func->functionType().parameterAtIndex(1).kind());
    ASSERT_EQ("b", func->functionType().parameterAtIndex(1).label());
}

TEST_F(ParserTests_Functions, FunctionWithOneReturn) {
    ASTNode* node = this->parseNode("def foo(; Int)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(0, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Integer, func->functionType().returnType().kind());
    ASSERT_EQ("", func->functionType().returnType().label());
}

TEST_F(ParserTests_Functions, FunctionWithTwoReturns) {
    ASTNode* node = this->parseNode("def foo(; Int, Float)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(0, func->functionType().parameterCount());

    ASSERT_EQ(NewDataType::Kind::Tuple, func->functionType().returnType().kind());
    ASSERT_EQ(2, func->functionType().returnType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().returnType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Kind::Float, func->functionType().returnType().subtypeAtIndex(1).kind());
}

TEST_F(ParserTests_Functions, FunctionWithTypedParamAndReturn) {
    ASTNode* node = this->parseNode("def foo(Int a; Int)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().returnAtIndex(0).kind());
    ASSERT_EQ("", func->functionType().returnAtIndex(0).label());
}

TEST_F(ParserTests_Functions, FunctionWithOneNamedReturn) {
    ASTNode* node = this->parseNode("def foo(; Int x)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(0, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Integer, func->functionType().returnType().kind());
    ASSERT_EQ("x", func->functionType().returnType().label());
}

TEST_F(ParserTests_Functions, DISABLED_FunctionNamedReturnIsALocal) {
    ASTNode* node = this->parseNode("def foo(; Int! x)\n"
                                    "  x = 1\n"
                                    "  return x\n"
                                    "end\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
}

TEST_F(ParserTests_Functions, FunctionWithFunctionParam) {
    ASTNode* node = this->parseNode("def foo(() a)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Kind::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
}

TEST_F(ParserTests_Functions, FunctionWithClosureParam) {
    ASTNode* node = this->parseNode("def foo({} a)\nend\n");

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Kind::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Closure, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("a", func->functionType().parameterAtIndex(0).label());
}

TEST_F(ParserTests_Functions, BackToBackEmptyFunctions) {
    Three::ASTNode* node = this->parseNode("def test()\n"
                                           "end\n"
                                           "def test2()\n"
                                           "end\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Function Definition", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Function Definition", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTests_Functions, MethodDefinition) {
    Three::ASTNode* node = this->parseNode("def Int.test()\nend\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Function Definition", node->nodeName());

    Three::FunctionDefinitionNode* func = dynamic_cast<Three::FunctionDefinitionNode*>(node);
    ASSERT_EQ("test", func->name());
    ASSERT_TRUE(func->isMethod());
    ASSERT_EQ(NewDataType::Kind::Integer, func->methodOnType().kind());
    
}

TEST_F(ParserTests_Functions, SelfInMethodDefinition) {
    Three::ASTNode* node = this->parseSingleFunction("def Int.test()\n"
                                                     "  *self + 1\n"
                                                     "end\n");

    ASSERT_EQ("Function Definition", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Addition Operator", node->nodeName());

    ASSERT_EQ("Integer Literal",  node->childAtIndex(1)->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Dereference Operator", node->nodeName());
    ASSERT_EQ("Self Variable", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Functions, Varargs) {
    Three::ASTNode* node = this->parseSingleFunction("def test(Vararg ap)\n"
                                                     "  Int a\n"
                                                     "  a = nextarg(Int, ap)\n"
                                                     "end\n");

    ASSERT_EQ("Function Definition", node->nodeName());

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node);

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("test", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Vararg, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ("ap", func->functionType().parameterAtIndex(0).label());
}

TEST_F(ParserTests_Functions, FunctionWithStructParam) {
    ASTNode* node = this->parseNode("struct Foo\n"
                                    "  Int a\n"
                                    "end\n"
                                    "def foo(Foo arg)\n"
                                    "end\n");

    ASSERT_EQ(2, node->childCount());
    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    FunctionDefinitionNode* func = dynamic_cast<FunctionDefinitionNode*>(node);
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ(NewDataType::Kind::Function, func->functionType().kind());
    ASSERT_EQ(0, func->functionType().subtypeCount());
    ASSERT_EQ(1, func->functionType().parameterCount());
    ASSERT_EQ(NewDataType::Kind::Void, func->functionType().returnType().kind());

    ASSERT_EQ(NewDataType::Kind::Structure, func->functionType().parameterAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, func->functionType().parameterAtIndex(0).access());
    ASSERT_EQ("arg", func->functionType().parameterAtIndex(0).label());
}
