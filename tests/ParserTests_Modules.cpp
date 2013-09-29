#include "ParserTestBase.h"

class ParserTest_Modules : public ParserTestBase {
};

TEST_F(ParserTest_Modules, ImportLinkageAndInclude) {
    ASTNode* node = this->parse("include \"time.h\"\nlinkage(\"pthread\")\ninclude \"pthread.h\"\n");

    ASSERT_EQ(3, node->childCount());

    ASSERT_INCLUDE("time.h", node->childAtIndex(0));
    ASSERT_LINKAGE("pthread", node->childAtIndex(1));
    ASSERT_INCLUDE("pthread.h", node->childAtIndex(2));
}

TEST_F(ParserTest_Modules, IncludeFollowedByNewlines) {
    ASTNode* node = this->parse("include \"pthread.h\"\n\n\ndef:type my_int Int\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_INCLUDE("pthread.h", node->childAtIndex(0));
    ASSERT_EQ("TypeDefinition", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, ModuleWithPublicType) {
    ASTNode* node = this->parse("public\ndef:type time_t Int\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Visibility", node->childAtIndex(0)->nodeName());

    TypeDefinitionNode* typeDef = dynamic_cast<TypeDefinitionNode*>(node->childAtIndex(1));
    ASSERT_EQ("TypeDefinition", typeDef->nodeName());
    ASSERT_EQ(TranslationUnit::Visibility::External, typeDef->visibility());
    ASSERT_EQ(0, typeDef->sourceType().indirectionDepth());
    ASSERT_EQ("Int", typeDef->sourceType().name());
    ASSERT_EQ("time_t", typeDef->destTypeName());
}

TEST_F(ParserTest_Modules, FunctionDeclarationUsingDefinedType) {
    ASTNode* node = this->parse("public\ndef:func foo(Int a; Void)\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, PrivateFunctionDeclaration) {
    ASTNode* node = this->parse("private\ndef:func foo()\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Visibility", node->childAtIndex(0)->nodeName());

    Three::FunctionDeclarationNode* funcDef = dynamic_cast<Three::FunctionDeclarationNode*>(node->childAtIndex(1));

    ASSERT_EQ("FunctionDeclaration", funcDef->nodeName());
    ASSERT_EQ(TranslationUnit::Visibility::None, funcDef->visibility());
}

TEST_F(ParserTest_Modules, FunctionDeclarationUsingDefinedStruct) {
    ASTNode* node = this->parse("struct MyStruct\nInt element\nend\ndef:func foo(MyStruct a)\n");

    ASSERT_EQ(2, node->childCount());

    ASSERT_STRUCT("MyStruct", 0, node->childAtIndex(0));
    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Modules, DefTypeForFunction) {
    ASTNode* node = this->parse("def:type func_t (*Void; Int)\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("TypeDefinition", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Modules, DefFuncWithEmptyFunctionAsParameter) {
    ASTNode* node = this->parse("def:func takes_func(() func)\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("FunctionDeclaration", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Modules, DefValueWithoutValue) {
    ASTNode* node = this->parse("def:value abc\n");

    ASSERT_EQ(1, node->childCount());

    ASSERT_EQ("ValueDefinition", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Modules, DefValueUsedForVariableInitialization) {
    ASTNode* node = this->parse("def:value abc\nInt x = abc\n");

    ASSERT_EQ("ValueDefinition", node->childAtIndex(0)->nodeName());

    Language::VariableDeclarationNode* varNode = dynamic_cast<Language::VariableDeclarationNode*>(node->childAtIndex(1));

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", varNode);
    ASSERT_EQ("Value", varNode->initializerExpression()->nodeName());
}

TEST_F(ParserTest_Modules, NamespaceBlock) {
    ASTNode* node = this->parse("namespace Something\ndef:func foo()\nend\n");

    ASSERT_EQ(1, node->childCount());

    Three::NamespaceNode* nsNode = dynamic_cast<Three::NamespaceNode*>(node->childAtIndex(0));

    ASSERT_EQ("Namespace", nsNode->nodeName());
    ASSERT_EQ("Something", nsNode->namespaceName());

    Three::FunctionDeclarationNode* defNode = dynamic_cast<Three::FunctionDeclarationNode*>(nsNode->childAtIndex(0));
    ASSERT_EQ("foo", defNode->function()->name());
    ASSERT_EQ("Something", defNode->function()->namespacePrefix());
}

TEST_F(ParserTest_Modules, DefinitionAfterNamespace) {
    ASTNode* node = this->parse("namespace Something\nend\ndef foo()\nend\n");

    std::cout << node->recursiveStr() << std::endl;

    ASSERT_EQ(2, node->childCount());

    ASSERT_EQ("Namespace", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("FunctionDefinition", node->childAtIndex(1)->nodeName());
}
