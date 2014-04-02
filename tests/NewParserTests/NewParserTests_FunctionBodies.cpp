#include "../NewParserTestsBase.h"

class NewParserTests_FunctionBodies : public NewParserTestsBase {
protected:
    FunctionDefinitionNode* parseFunction(const char* input) {
        ASTNode* node = this->parseNode(input);

        return dynamic_cast<FunctionDefinitionNode*>(node->childAtIndex(0));
    }
};

TEST_F(NewParserTests_FunctionBodies, EmptyFunction) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\nend\n");

    ASSERT_EQ("Function Definition", func->nodeName());
    ASSERT_EQ("foo", func->name());

    ASSERT_EQ("", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, FunctionWithComment) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  # abc def\n"
                                                       "end\n");

    ASSERT_EQ("  \n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, Statements) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  a = b\n"
                                                       "  Type d = b\n"
                                                       "end\n");

    // TODO: the leading whitespace is dropped here...
    ASSERT_EQ("  a = b\n  Type d = b\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, If) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  if abc\n"
                                                       "  end\n"
                                                       "end\n");

    ASSERT_EQ("  if abc\n  end\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, TailingIf) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  return if abc\n"
                                                       "end\n");

    ASSERT_EQ("  return if abc\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, AtomicStatement) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  atomic\n"
                                                       "  end\n"
                                                       "end\n");

    ASSERT_EQ("  atomic\n  end\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, AtomicExpression) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  atomic(x += 1)\n"
                                                       "end\n");

    ASSERT_EQ("  atomic(x += 1)\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, AtomicExpressionWithSpace) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  atomic (x += 1)\n"
                                                       "end\n");

    ASSERT_EQ("  atomic (x += 1)\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, AtomicExpressionWithOrdering) {
    FunctionDefinitionNode* func = this->parseFunction("def foo()\n"
                                                       "  atomic:relaxed(x += 1)\n"
                                                       "end\n");

    ASSERT_EQ("  atomic:relaxed(x += 1)\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, CASOperator) {
    FunctionDefinitionNode* func = this->parseFunction("def test()\n"
                                                       "  a cas b : c\n"
                                                       "end\n");

    ASSERT_EQ("  a cas b : c\n", func->bodyStream()->str());
}

TEST_F(NewParserTests_FunctionBodies, CASOperatorWithType) {
    FunctionDefinitionNode* func = this->parseFunction("def test(Int a)\n"
                                                       "  a cas b : c\n"
                                                       "end\n");

    ASSERT_EQ("  a cas b : c\n", func->bodyStream()->str());
}
