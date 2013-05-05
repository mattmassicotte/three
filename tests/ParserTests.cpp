#include "compiler/Parser.h"

#include <assert.h>
#include <gtest/gtest.h>

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

TEST_F(ParserTest, SingleIdentifierWithDigit) {
    Language::ASTNode* node;
    
    node = this->parse("def main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n end\n");

    std::cout << node->recursiveStr() << std::endl;

    ASSERT_TRUE(true);
}
