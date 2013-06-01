#include "Parser.h"
#include "AST/RootNode.h"
#include "AST/FunctionDefinitionNode.h"
#include "AST/FunctionCallNode.h"
#include "AST/StringLiteralNode.h"
#include "AST/ReturnNode.h"
#include "AST/IntegerLiteralNode.h"
#include "AST/IfNode.h"
#include "AST/BooleanLiteralNode.h"
#include "AST/ImportNode.h"
#include "AST/VariableDeclarationNode.h"
#include "AST/VariableNode.h"
#include "AST/OperatorNode.h"

#include <assert.h>

#define DEBUG_PARSING 0

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
        _currentScope = Scope::createRootScope();
        _rootModule = Module::createRootModule();
    }

    Parser::~Parser() {
        delete _currentScope;
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
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: statement '" << this->peek().str() << "'" << std::endl;
#endif

        // TODO: This really needs to do more exhausive checks for a variable
        // declaration.

        switch (this->peek().type()) {
            case Token::Type::Identifier:
                if (this->peek(2).type() == Token::Type::Identifier) {
                    node = VariableDeclarationNode::parse(*this);
                }
                break;
            case Token::Type::PunctuationOpenBrace:
                // closure type
                node = VariableDeclarationNode::parse(*this);
                break;
            case Token::Type::KeywordReturn:
                node = ReturnNode::parse(*this);
                break;
            case Token::Type::KeywordIf:
                node = IfNode::parse(*this);
                break;
            default:
                break;
        }

        if (!node) {
#if DEBUG_PARSING
            std::cout << "Parser: falling back to expression statement" << std::endl;
#endif
            node = this->parseExpression();
        }

        assert(this->next().type() == Token::Type::Newline);
        node->setStatement(true);

        return node;
    }

    ASTNode* Parser::parseExpression() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: expression: '" << this->peek().str() << "'" << std::endl;
#endif

        // TODO: clean this up!
        if (this->nextIf("(")) {
            node = this->parseExpression();

            assert(this->next().str() == ")");
        } else {
        switch (this->peek().type()) {
            case Token::Type::Identifier:
                if (this->peek(2).str().at(0) == '(') {
                    node = FunctionCallNode::parse(*this);
                } else {
                    node = VariableNode::parse(*this);
                }
                break;
            case Token::Type::String:
                node = StringLiteralNode::parse(*this);
                break;
            case Token::Type::NumericLiteral:
                node = IntegerLiteralNode::parse(*this);
                break;
            case Token::Type::TrueLiteral:
            case Token::Type::FalseLiteral:
                node = BooleanLiteralNode::parse(*this);
                break;
            default:
                std::cout << "Parser: unhandled expression '" << this->next().str() << "'" << std::endl;
                break;
        }
    }

        assert(node != NULL);

        return OperatorNode::parse(*this, Token::MinimumPrecedence, node);
    }

    TypeReference Parser::parseType() {
        DataType* dataType = NULL;
        uint32_t  depth    = 0;

        // parse '*', if they are there
        while (this->nextIf("*")) {
            depth++;
        }

        // parse the type
        switch (this->peek().type()) {
            case Token::Type::Identifier:
                dataType = this->currentModule()->dataTypeForName(this->next().str());
                break;
            case Token::Type::PunctuationOpenBrace:
                return this->parseFunctionType(depth);
                break;
            default:
                assert(0 && "Got to a weird value parsing a type");
                break;
        }

        return TypeReference(dataType, depth);
    }

    TypeReference Parser::parseFunctionType(uint32_t depth) {
        DataType* type;
        bool      closure;

        // determine if we are parsing a function type, a closure type, or we have an error
        switch (this->peek().str().at(0)) {
            case '(':
                closure = false;
                type = new DataType(DataType::Flavor::Function, "");
                break;
            case '{':
                closure = true;
                type = new DataType(DataType::Flavor::Closure, "");
                break;
            default:
                assert(0 && "Trying to parse a function type, but didn't find an opening '(' or '{'");
                break;
        }

        this->next(); // advance past the opening punctuation

        while (true) {
            Token t = this->peek();

            if (t.str().at(0) == ';' || t.str().at(0) == ')' || t.str().at(0) == '}') {
                break;
            }

            type->addChild(this->parseType());

            // here is where labels could be parsed
            assert(this->peek().type() != Token::Type::Identifier && "Label parsing?");

            // a ',' means another paramter was specified
            if (!this->nextIf(",")) {
                break;
            }
        }
        
        // a function
        // - )
        // - ;Type)
        
        if (!closure) {
            if (this->nextIf(")")) {
                type->setReturnType(TypeReference(this->currentModule()->dataTypeForName("Void"), 0));
            } else {
                assert(this->nextIf(";"));
                type->setReturnType(this->parseType());
                assert(this->nextIf(")"));
            }
        
            return TypeReference(type, depth);
        }
        
        // ok, so we have a closure.
        // - }
        // - ;Type}
        // - ;;capture list}
        // - ;Type;capture list}
        
        if (this->nextIf("}")) {
            type->setReturnType(TypeReference(this->currentModule()->dataTypeForName("Void"), 0));
            return TypeReference(type, depth);
        }
        
        // return type seperator
        assert(this->nextIf(";"));
        
        // return type
        if (!this->nextIf(";")) {
            type->setReturnType(this->parseType());
        }
        
        // is there a capture list
        if (this->nextIf("}")) {
            return TypeReference(type, depth);
        }
        
        // type._returnType = DataType(this->currentModule()->dataStructureForName("Void"), 0);
        // 
        // // capture list
        // if (!references) {
        //     return type;
        // }
        // 
        // // loop over the capture list
        // while (true) {
        //     Token t = this->peek();
        //     if (t.str().at(0) == '}') {
        //         break;
        //     }
        // 
        //     assert(t.type() == Token::Type::Identifier);
        //     if (references) {
        //         references->push_back(t.str());
        //     }
        //     this->next();
        // 
        //     if (!this->nextIf(",")) {
        //         break;
        //     }
        // }
        // 
        return TypeReference(type, depth);
    }

    Module* Parser::moduleWithIdentifier(const std::string& name) {
        Module* module = _module[name];

        if (!module) {
            module = Module::createModule(_rootModule, name, std::vector<std::string>());
            _rootModule->addChild(module);
        }

        return module;
    }

    Module* Parser::currentModule() const {
        return _rootModule;
    }

    ASTNode* Parser::parseTopLevelNode() {
        // std::cout << "Parser: top level" << std::endl;
        
        switch (this->peek().type()) {
            case Token::Type::KeywordDef:
                return this->parseDefinition();
            case Token::Type::KeywordImport:
                return ImportNode::parse(*this);
            case Token::Type::EndOfInput:
                assert(0 && "parseTopLevelNode invalid state");
                break;
            default:
                this->next();
                return new RootNode();
        }

        return NULL;
    }

    ASTNode* Parser::parseDefinition() {
        // std::cout << "Parser: definition" << std::endl;

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

    bool Parser::nextIf(const std::string& value) {
        return _lexer->nextIf(value);
    }

    Scope* Parser::currentScope() const {
        assert(_currentScope);

        return _currentScope;
    }

    void Parser::pushScope(Scope* scope) {
        assert(scope);

        this->currentScope()->setParent(scope);
        _currentScope = scope;
    }

    void Parser::popScope() {
        Scope* old = this->currentScope();

        _currentScope = old->parent();
        assert(_currentScope);

        delete old;
    }
}
