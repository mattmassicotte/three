#include "Parser.h"
#include "AST/RootNode.h"
#include "AST/FunctionDefinitionNode.h"
#include "AST/FunctionCallNode.h"

#include <assert.h>

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
    }

    ASTNode* Parser::rootASTNode() {
        RootNode* root;

        root = new RootNode();

        while (!this->peek().isEnd()) {
            root->addChild(this->parseTopLevelNode());
        }

        return root;
    }

    ASTNode* Parser::parseStatement() {
        std::cout << "Parser: statement" << std::endl;

        switch (this->peek().type()) {
            case Token::Type::Identifier:
                if (this->peek(2).str().at(0) == '(') {
                    return FunctionCallNode::parse(*this);
                }
            default:
                std::cout << "Parser: unhandled '" << this->next().str() << "'" << std::endl;
                break;
        }

        return new RootNode();
    }

    ASTNode* Parser::parseTopLevelNode() {
        std::cout << "Parser: top level" << std::endl;
        
        switch (this->peek().type()) {
            case Token::Type::KeywordDef:
                return this->parseDefinition();
            default:
                this->next();
                return new RootNode();
        }

        return NULL;
    }

    ASTNode* Parser::parseDefinition() {
        std::cout << "Parser: definition" << std::endl;

        // the kinds of definitions we have are:
        // - a function body
        // - a function prototype
        // - a type

        assert(this->next().type() == Token::Type::KeywordDef); // skip over the "def"

        if (this->peek(2).str() == "(") {
            return FunctionDefinitionNode::parse(*this);
        }

        return new RootNode();
    }

    Token Parser::peek(unsigned int distance) {
        return _lexer->peek(distance);
    }

    Token Parser::next() {
        return _lexer->next();
    }
}
