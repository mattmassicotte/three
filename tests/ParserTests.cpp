#include "compiler/Parser.h"
#include "compiler/AST.h"

#include <assert.h>
#include <gtest/gtest.h>

#define ASSERT_IMPORT(path_value, node) do {\
Language::ImportNode* tmp = dynamic_cast<Language::ImportNode*>(node); \
ASSERT_EQ("Import", tmp->name()); \
ASSERT_EQ(path_value, tmp->path()); \
} while(0)

#define ASSERT_STRING_LITERAL_NODE(str_value, node) do {\
Language::StringLiteralNode* tmp = dynamic_cast<Language::StringLiteralNode*>(node); \
ASSERT_EQ("StringLiteral", tmp->name()); \
ASSERT_EQ(str_value, tmp->stringValue()); \
} while(0)

#define ASSERT_INTEGER_LITERAL_NODE(num_value, node) do {\
Language::IntegerLiteralNode* tmp = dynamic_cast<Language::IntegerLiteralNode*>(node); \
ASSERT_EQ("IntegerLiteral", tmp->name()); \
ASSERT_EQ(num_value, tmp->value()); \
} while(0)

#define ASSERT_BOOLEAN_LITERAL_NODE(bool_value, node) do { \
Language::BooleanLiteralNode* tmp = dynamic_cast<Language::BooleanLiteralNode*>(node); \
ASSERT_EQ("BooleanLiteral", tmp->name()); \
ASSERT_EQ(bool_value, tmp->value()); \
} while(0)

#define ASSERT_RETURN_NODE(node) do {\
Language::ReturnNode* tmp = dynamic_cast<Language::ReturnNode*>(node); \
ASSERT_EQ("Return", tmp->name()); \
} while(0)

#define ASSERT_OPERATOR(op_value, node) do { \
Language::OperatorNode* tmp = dynamic_cast<Language::OperatorNode*>(node); \
ASSERT_EQ("Operator", tmp->name()); \
ASSERT_EQ(op_value, tmp->op()); \
} while(0)

#define ASSERT_DATA_TYPE(dt_name, obj) do {\
ASSERT_EQ(dt_name, obj->name()); \
} while(0)

#define ASSERT_VARIABLE(var_type, var_indirection, var_name, obj) do { \
Language::Variable* tmp = obj; \
ASSERT_DATA_TYPE(var_type, tmp->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->name()); \
} while(0)

#define ASSERT_VARIABLE_NODE(var_type, var_indirection, var_name, obj) do {\
Language::VariableNode* tmp = dynamic_cast<Language::VariableNode*>(obj); \
ASSERT_EQ("Variable", tmp->name()); \
ASSERT_DATA_TYPE(var_type, tmp->variable()->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->variable()->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->variable()->name()); \
} while(0)

#define ASSERT_VARIABLE_DECLERATION(var_type, var_indirection, var_name, obj) do {\
Language::VariableDeclarationNode* tmp = dynamic_cast<Language::VariableDeclarationNode*>(obj); \
ASSERT_EQ("VariableDeclaration", tmp->name()); \
ASSERT_DATA_TYPE(var_type, tmp->variable()->type().referencedType()); \
ASSERT_EQ(var_indirection, tmp->variable()->type().indirectionDepth()); \
ASSERT_EQ(var_name, tmp->variable()->name()); \
} while(0)

class ParserTest : public testing::Test {
protected:
    virtual void SetUp() {
        _stream = NULL;
        _lexer  = NULL;
        _parser = NULL;
    }

    virtual void TearDown() {
        delete _parser;
        delete _lexer;
        delete _stream;
    }

    Language::ASTNode* parse(const char* input) {
        std::string inputString(input);

        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Language::Lexer(_stream);

        assert(!_parser);
        _parser = new Language::Parser(_lexer);

        return _parser->rootASTNode();
    }

    Language::ASTNode* parseString() {
        return _parser->rootASTNode();
    }

    std::istringstream* _stream;
    Language::Lexer*    _lexer;
    Language::Parser*   _parser;
};

TEST_F(ParserTest, SimpleHelloWorldProgram) {
    Language::ASTNode* node;

    node = this->parse("import C.stdio\n\ndef main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n    return 0\nend\n");

    ASSERT_EQ("Root", node->name());
    ASSERT_EQ(2, node->childCount());

    // import statement
    ASSERT_IMPORT("C.stdio", node->childAtIndex(0));

    // function definition
    Language::FunctionDefinitionNode* defNode;

    defNode = (Language::FunctionDefinitionNode *)node->childAtIndex(1);

    ASSERT_EQ("FunctionDefinition", defNode->name());
    ASSERT_EQ(2, defNode->childCount());
    ASSERT_EQ("main", defNode->function()->name());

    ASSERT_VARIABLE("Int",  0, "argc", defNode->function()->parameterAtIndex(0));
    ASSERT_VARIABLE("Char", 2, "argv", defNode->function()->parameterAtIndex(1));
    // TODO: fix this
    // ASSERT_DATA_TYPE("Int",  0, defNode->function()->returnType());

    Language::FunctionCallNode* callNode;

    callNode = (Language::FunctionCallNode *)defNode->childAtIndex(0);

    ASSERT_EQ("FunctionCall", callNode->name());
    ASSERT_EQ("printf", callNode->functionName());
    ASSERT_EQ(1, callNode->childCount());

    ASSERT_STRING_LITERAL_NODE("hello world", callNode->childAtIndex(0));

    // return statement
    Language::ReturnNode* returnNode = dynamic_cast<Language::ReturnNode*>(defNode->childAtIndex(1));

    ASSERT_RETURN_NODE(returnNode);
    ASSERT_EQ(1, returnNode->childCount());
    ASSERT_INTEGER_LITERAL_NODE(0, returnNode->childAtIndex(0));
}

TEST_F(ParserTest, SimpleIfWithNoElseStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nif true\nreturn\nend\nend\n");

    Language::IfNode* ifNode = dynamic_cast<Language::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(true, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_EQ(NULL, ifNode->elseStatement());
}

TEST_F(ParserTest, SimpleIfWithElseStatement) {
    Language::ASTNode* node;

    node = this->parse("def test()\nif false\nreturn\nelse\nreturn\nend\nend\n");

    Language::IfNode* ifNode = dynamic_cast<Language::IfNode*>(node->childAtIndex(0)->childAtIndex(0));

    ASSERT_EQ("If", ifNode->name());
    ASSERT_BOOLEAN_LITERAL_NODE(false, ifNode->condition());
    ASSERT_EQ(1, ifNode->childCount());
    ASSERT_RETURN_NODE(ifNode->childAtIndex(0));
    ASSERT_RETURN_NODE(ifNode->elseStatement()->childAtIndex(0));
}

TEST_F(ParserTest, AssignmentExpression) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt x\nx = 42\nend\n");

    Language::OperatorNode* opNode = dynamic_cast<Language::OperatorNode*>(node->childAtIndex(0)->childAtIndex(1));

    ASSERT_OPERATOR("=", opNode);
    ASSERT_VARIABLE_NODE("Int",  0, "x", opNode->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(42, opNode->childAtIndex(1));
}

TEST_F(ParserTest, ComplexExpression) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt x\nx = 42 * (5 + 1)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));

    Language::OperatorNode* opNode = dynamic_cast<Language::OperatorNode*>(node->childAtIndex(1));

    ASSERT_OPERATOR("=", opNode);
    ASSERT_VARIABLE_NODE("Int",  0, "x", opNode->childAtIndex(0));

    opNode = dynamic_cast<Language::OperatorNode*>(opNode->childAtIndex(1));
    ASSERT_OPERATOR("*", opNode);
    ASSERT_INTEGER_LITERAL_NODE(42, opNode->childAtIndex(0));

    opNode = dynamic_cast<Language::OperatorNode*>(opNode->childAtIndex(1));
    ASSERT_OPERATOR("+", opNode);
    ASSERT_INTEGER_LITERAL_NODE(5, opNode->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, opNode->childAtIndex(1));
}

TEST_F(ParserTest, ClosureInFunction) {
    Language::ASTNode* node;

    node = this->parse("import C.stdio\n\ndef main(;Int)\nInt x\nInt y\n{Int, Int; Int} closure\nx = 42\ny = 43\nclosure = do (Int a, Int b; Int; x) {\nx = a + b + y\nreturn a + b\n}\n printf(\"x = %d, value = %d\\n\", x, closure(1,2))\nprintf(\"x = %d\\n\", x)\nend\n");

    // import statement
    ASSERT_IMPORT("C.stdio", node->childAtIndex(0));

    node = node->childAtIndex(1);

    // Variable declarations
    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));
    ASSERT_VARIABLE_DECLERATION("Int", 0, "y", node->childAtIndex(1));

    Language::VariableDeclarationNode* varDecl = dynamic_cast<Language::VariableDeclarationNode*>(node->childAtIndex(2));
    ASSERT_VARIABLE_DECLERATION("", 0, "closure", varDecl);
    ASSERT_EQ(2, varDecl->variable()->type().referencedType()->childCount());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(0).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(0).indirectionDepth());
    ASSERT_EQ("Int", varDecl->variable()->type().referencedType()->childAtIndex(1).name());
    ASSERT_EQ(0, varDecl->variable()->type().referencedType()->childAtIndex(1).indirectionDepth());

    ASSERT_OPERATOR("=", node->childAtIndex(3));
    ASSERT_OPERATOR("=", node->childAtIndex(4));
    ASSERT_OPERATOR("=", node->childAtIndex(5));

    Language::ClosureNode* closure = dynamic_cast<Language::ClosureNode*>(node->childAtIndex(5)->childAtIndex(1));
    ASSERT_EQ("Closure", closure->name());
}

TEST_F(ParserTest, NewlinesAfterStatements) {
    Language::ASTNode* node;

    node = this->parse("def test()\n\nInt x\n\nx = 1\n\n\nend\n\n\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLERATION("Int", 0, "x", node->childAtIndex(0));

    node = node->childAtIndex(1);

    ASSERT_OPERATOR("=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "x", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest, StructureDefinitionWithPacking) {
    Language::ASTNode* node;

    node = this->parse("struct:2 MyStructure\nInt x\nInt y\nend\n");

    Language::StructureNode* structure = dynamic_cast<Language::StructureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Structure", structure->name());
    ASSERT_EQ(2, structure->packing());
    ASSERT_EQ("MyStructure", structure->structureName());
}

TEST_F(ParserTest, StructureDefinitionWithoutPacking) {
    Language::ASTNode* node;

    node = this->parse("struct MyStructure\nInt x\nInt y\nend\n");

    Language::StructureNode* structure = dynamic_cast<Language::StructureNode*>(node->childAtIndex(0));
    ASSERT_EQ("Structure", structure->name());
    ASSERT_EQ(0, structure->packing());
    ASSERT_EQ("MyStructure", structure->structureName());
}

TEST_F(ParserTest, AtomicBarrierWithoutSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->name());
    ASSERT_EQ("ordered", dynamic_cast<Language::BarrierNode*>(node)->type());
}

TEST_F(ParserTest, AtomicBarrierWithSpecifier) {
    Language::ASTNode* node;

    node = this->parse("def test()\nbarrier:unordered\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_EQ("Barrier", node->name());
    ASSERT_EQ("unordered", dynamic_cast<Language::BarrierNode*>(node)->type());
}

TEST_F(ParserTest, AtomicExpression) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt a\natomic:ordered(a = 1)\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_EQ("AtomicExpression", node->name());
    ASSERT_EQ("ordered", dynamic_cast<Language::AtomicExpressionNode*>(node)->type());
}

TEST_F(ParserTest, TernaryConditionalOperator) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt a\na = (a == 0) ? 1 : 2\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("=", node);
    ASSERT_EQ(2, node->childCount());
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));

    node = node->childAtIndex(1);
    ASSERT_OPERATOR("?", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
    ASSERT_INTEGER_LITERAL_NODE(2, node->childAtIndex(2));

    node = node->childAtIndex(0);
    ASSERT_OPERATOR("==", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));
}

TEST_F(ParserTest, TernaryCompareAndSwapOperator) {
    Language::ASTNode* node;

    node = this->parse("def test()\nInt a\na cas 1 : 2\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("cas", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
    ASSERT_INTEGER_LITERAL_NODE(2, node->childAtIndex(2));
}
