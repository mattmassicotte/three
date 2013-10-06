#include "compiler/Parser.h"
#include "ParserAsserts.h"

class ParserTestBase : public testing::Test {
protected:
    virtual void SetUp() {
        _stream  = NULL;
        _lexer   = NULL;
        _parser  = NULL;
        _context = NULL;
    }

    virtual void TearDown() {
        delete _context;
        delete _parser;
        delete _lexer;
        delete _stream;
    }

    Three::ASTNode* parse(const char* input) {
        std::string inputString(input);

        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Three::Lexer(_stream);

        assert(!_parser);
        _parser = new Three::Parser(_lexer);

        assert(!_context);
        _context = Three::ParsingContext::translationUnitContext();

        _parser->setContext(_context);

        return _parser->rootASTNode();
    }

    Three::ASTNode* parseString() {
        return _parser->rootASTNode();
    }

    std::istringstream* _stream;
    Three::Lexer*    _lexer;
    Three::Parser*   _parser;
    Three::ParsingContext* _context;
};
