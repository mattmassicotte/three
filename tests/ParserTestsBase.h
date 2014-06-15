#include "compiler/Lexer/Lexer.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/Messages/Message.h"

class ParserTestsBase : public testing::Test {
protected:
    virtual void SetUp() {
        _skipIncludes = false;
        _skipImports = false;

        _stream  = nullptr;
        _lexer   = nullptr;
        _parser  = nullptr;
        _context = nullptr;
    }

    virtual void TearDown() {
        delete _context;
        delete _parser;
        delete _lexer;
        delete _stream;
    }

    Three::ParseContext* parse(const char* input) {
        std::string inputString(input);

        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Three::Lexer(_stream);

        assert(!_parser);
        _parser = new Three::NewParser();

        assert(!_context);
        _context = new Three::ParseContext();
        _context->skipIncludes = _skipIncludes;
        _context->skipImports = _skipImports;

        _parser->parse(_lexer, _context);

        // handle messages
        for (int i = 0; i < _context->messageCount(); ++i) {
            std::cout << _context->messageAtIndex(i)->str() << std::endl;
        }

        if (_context->messageCount() > 0) {
            return nullptr;
        }

        return _context;
    }

    Three::ParseContext* parseWithBodies(const char* input) {
        this->parse(input);

        _parser->parseFunctionBodies(_context);

        return _context;
    }

    Three::ASTNode* parseNode(const char* input) {
        return this->parse(input)->rootNode();
    }

    Three::ASTNode* parseNodeWithBodies(const char* input) {
        return this->parseWithBodies(input)->rootNode();
    }

    Three::FunctionDefinitionNode* parseSingleFunction(const char* input) {
        return dynamic_cast<FunctionDefinitionNode*>(this->parseNodeWithBodies(input)->childAtIndex(0));
    }

    Three::ParseContext* context() const {
        return _context;
    }

    void setSkipIncludes(bool value) {
        _skipIncludes = value;
    }

    void setSkipImports(bool value) {
        _skipImports = value;
    }

    std::istringstream* _stream;
    Three::Lexer* _lexer;
    Three::NewParser* _parser;
    Three::ParseContext* _context;
    bool _skipIncludes;
    bool _skipImports;
};

