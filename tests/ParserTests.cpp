#include "compiler/Parser.h"
#include "compiler/AST/FunctionDefinitionNode.h"
#include "compiler/AST/FunctionCallNode.h"
#include "compiler/AST/DataType.h"
#include "compiler/AST/StringLiteralNode.h"
#include "compiler/AST/ReturnNode.h"
#include "compiler/AST/IntegerLiteralNode.h"

#include <assert.h>
#include <gtest/gtest.h>

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

#define ASSERT_RETURN_NODE(node) do {\
Language::ReturnNode* tmp = dynamic_cast<Language::ReturnNode*>(node); \
ASSERT_EQ("Return", tmp->name()); \
} while(0)

#define ASSERT_DATA_TYPE(dt_name, dt_indirection, obj) do {\
ASSERT_EQ(dt_name, obj.typeName()); \
ASSERT_EQ(dt_indirection, obj.indirectionDepth()); \
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

    node = this->parse("def main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n    return 0\nend\n");

    ASSERT_EQ("Root", node->name());
    ASSERT_EQ(1, node->childCount());

    Language::FunctionDefinitionNode* defNode;

    defNode = (Language::FunctionDefinitionNode *)node->childAtIndex(0);

    ASSERT_EQ("FunctionDefinition", defNode->name());
    ASSERT_EQ(2, defNode->childCount());
    ASSERT_EQ("main", defNode->functionName());

    ASSERT_DATA_TYPE("Int",  0, defNode->parameters().at(0));
    ASSERT_DATA_TYPE("Char", 2, defNode->parameters().at(1));
    ASSERT_DATA_TYPE("Int",  0, defNode->returnType());

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

    std::stringstream s;

    node->renderCCode(s);

    std::cout << s.str() << std::endl;

}
