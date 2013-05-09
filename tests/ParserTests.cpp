#include "compiler/Parser.h"
#include "compiler/AST/FunctionDefinitionNode.h"
#include "compiler/AST/FunctionCallNode.h"
#include "compiler/AST/DataType.h"
#include "compiler/AST/StringLiteralNode.h"

#include <assert.h>
#include <gtest/gtest.h>

#define ASSERT_TOKEN(exp_type, exp_value, token) do {\
Language::Token tmp = token; \
EXPECT_EQ(exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
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

    node = this->parse("def main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n end\n");

    ASSERT_EQ("Root", node->name());
    ASSERT_EQ(1, node->childCount());

    Language::FunctionDefinitionNode* defNode;

    defNode = (Language::FunctionDefinitionNode *)node->childAtIndex(0);

    ASSERT_EQ("FunctionDefinition", defNode->name());
    ASSERT_EQ(1, defNode->childCount());
    ASSERT_EQ("main", defNode->functionName());
    
    ASSERT_EQ("Int", defNode->parameters().at(0).typeName());
    ASSERT_EQ(0, defNode->parameters().at(0).indirectionDepth());
    ASSERT_EQ("Char", defNode->parameters().at(1).typeName());
    ASSERT_EQ(2, defNode->parameters().at(1).indirectionDepth());
    ASSERT_EQ("Int", defNode->returnType().typeName());
    ASSERT_EQ(0, defNode->returnType().indirectionDepth());

    Language::FunctionCallNode* callNode;

    callNode = (Language::FunctionCallNode *)defNode->childAtIndex(0);

    ASSERT_EQ("FunctionCall", callNode->name());
    ASSERT_EQ("printf", callNode->functionName());
    ASSERT_EQ(1, callNode->childCount());

    Language::StringLiteralNode* stringNode = dynamic_cast<Language::StringLiteralNode*>(callNode->childAtIndex(0));

    ASSERT_EQ("StringLiteral", stringNode->name());
    ASSERT_EQ("hello world", stringNode->stringValue());
}
