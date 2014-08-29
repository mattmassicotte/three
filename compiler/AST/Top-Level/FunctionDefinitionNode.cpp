#include "FunctionDefinitionNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/Lexer/Lexer.h"
#include "compiler/constructs/NewScope.h"
#include "compiler/AST/Atomics/AtomicNode.h"

#include <assert.h>
#include <sstream>

namespace Three {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordDef));

        FunctionDefinitionNode* node = new FunctionDefinitionNode();

        node->setVisibility(parser.context()->visibility());
        node->namespaceComponents = parser.context()->scope()->fullNamespace();

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: Expected function name or method type");
        }

        // we have a method
        if (parser.helper()->peek(2).type() == Token::Type::OperatorDot) {
            if (!parser.context()->definesTypeWithName(parser.helper()->peek().str())) {
                assert(0 && "Message: Method found, but type not recognized");
            }

            node->_methodOnType = parser.context()->dataTypeForName(parser.helper()->nextStr());

            assert(parser.helper()->nextIf(Token::Type::OperatorDot));
        }

        // sanity check the type we are at, and make sure we can actually
        // use the name selected
        assert(parser.helper()->peek().type() == Token::Type::Identifier);
        if (!parser.isAtIdentifierAvailableForDefinition()) {
            assert(0 && "Message: function name isn't avaiable for definition");
        }

        node->_name = parser.helper()->nextStr();

        // parse signature
        node->_functionType = parser.parseFunctionSignatureType();
        node->_functionType.setLabel(node->fullName());

        // insert the self parameter, if needed
        if (node->isMethod()) {
            NewDataType selfPtr = NewDataType::wrapInPointer(node->_methodOnType);
            selfPtr.setLabel("self");

            node->_functionType.parameters.insert(node->_functionType.parameters.cbegin(), selfPtr);
        }

        if (!parser.context()->defineFunctionForName(node->_functionType, node->_name)) {
            assert(0 && "Message: function name already used");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: A newline should appear after a function definition signature");
        }

        if (!FunctionDefinitionNode::bufferFunctionBody(parser, node->_bodyString)) {
            assert(0 && "Message: Unable to buffer function body");
        }

        if (!parser.helper()->nextIf(Token::Type::KeywordEnd)) {
            assert(0 && "Message: A function must be closed with the end keyword");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: A newline should appear after end keyword");
        }

        parser.addFunctionForParsing(node);

        return node;
    }

    bool FunctionDefinitionNode::bufferFunctionBody(Parser& parser, std::stringstream& stream) {
        parser.helper()->lexer()->setFilterWhitespace(false);

        bool result = bufferOpenToCloseToken(parser, stream, Token::Type::KeywordEnd, false);

        parser.helper()->lexer()->setFilterWhitespace(true);

        return result;
    }

    // This is a really complex function. The idea here is to scan all the tokens inside a function body,
    // making sure to detect the closing "end" statement. Some features of the language make this particularly
    // hard.
    //
    // The basic idea is, buffer up tokens, and if you find a token that gets paired with an 'end',
    // recurse and continue.
    bool FunctionDefinitionNode::bufferOpenToCloseToken(Parser& parser, std::stringstream& stream, Token::Type closingType, bool parseClosing) {
        bool firstCall = !parseClosing;

        for (;;) {
            bool potentialFirstToken = scanToFirstToken(parser, stream, firstCall);

            // can only be the first call once
            if (firstCall) {
                firstCall = false;
            }

            Token t = parser.helper()->peek();

            // maybe we're done?
            if (t.type() == closingType) {
                if (parseClosing) {
                    stream << parser.helper()->nextStr();
                }

                return true;
            }

            // are we starting a new block?
            // Note that the atomic keyword is ambigious here, so we have to check for that more carefully
            if (potentialFirstToken && t.isOpeningSpan() && !AtomicNode::isAtAtomicExpression(parser)) {
                Token::Type nestedType = t.closingCounterpart();
                if (nestedType == Token::Type::Undefined) {
                    return false;
                }

                stream << parser.helper()->nextStr(); // move past the opening

                // recurse!
                if (!bufferOpenToCloseToken(parser, stream, nestedType, true)) {
                    return false;
                } else {
                    continue; // restart the loop and keep going
                }
            }

            // look for unexpected types that should cause us to stop
            switch (t.type()) {
                case Token::Type::KeywordDef:
                case Token::Type::Undefined:
                case Token::Type::EndOfInput:
                    return false;
                default:
                    break;
            }

            // buffer and continue
            stream << parser.helper()->nextStr();
        }
    }

    bool FunctionDefinitionNode::scanToFirstToken(Parser& parser, std::stringstream& stream, bool firstCall) {
        // if firstCall is true, we've been called immediately after a function definition line,
        // and there will be no newline
        if (!firstCall) {
            if (parser.helper()->peek().type() != Token::Type::Newline) {
                return false;
            }

            stream << parser.helper()->nextStr();
        }

        // now, just scan for whitespace, and if we find any, we have a potential first token
        bool potentialFirst = false;
        while (parser.helper()->peek().type() == Token::Type::Whitespace) {
            stream << parser.helper()->nextStr();
            potentialFirst = true;
        }

        return potentialFirst;
    }

    std::string FunctionDefinitionNode::nodeName() const {
        return "Function Definition";
    }

    std::string FunctionDefinitionNode::name() const {
        return _name;
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << "Function: " << this->name();

        return s.str();
    }

    void FunctionDefinitionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    NewDataType FunctionDefinitionNode::functionType() const {
        return _functionType;
    }

    NewDataType FunctionDefinitionNode::methodOnType() const {
        return _methodOnType;
    }
    
    bool FunctionDefinitionNode::isMethod() const {
        return _methodOnType.kind() != NewDataType::Kind::Undefined;
    }

    std::stringstream* FunctionDefinitionNode::bodyStream() {
        return &_bodyString;
    }

    void FunctionDefinitionNode::defineVariableForReturnType(NewScope* scope, const NewDataType& type) const {
        if (type.label().size() == 0) {
            return;
        }

        scope->defineVariableTypeForName(type, type.label());
    }

    void FunctionDefinitionNode::defineParameterVariablesInScope(NewScope* scope) const {
        // create variables for all arguments in this scope, including the special self
        // variable if this is a method
        _functionType.eachParameterWithLast([&] (const NewDataType& type, bool last) {
            scope->defineVariableTypeForName(type, type.label());
        });

        // now, create variables for each named return. And, we have to treat tuples
        // special
        if (_functionType.returnType().kind() == NewDataType::Kind::Tuple) {
            _functionType.returnType().eachSubtypeWithLast([&] (const NewDataType& type, bool last) {
                this->defineVariableForReturnType(scope, type);
            });
        } else {
            this->defineVariableForReturnType(scope, _functionType.returnType());
        }
    }

    bool FunctionDefinitionNode::parseBody(Parser& parser) {
        parser.context()->pushScope();
        parser.context()->scope()->setScopedBasename(this->name());

        this->defineParameterVariablesInScope(parser.context()->scope());

        parser.helper()->parseUntil(false, [&] (const Token& token) {
            ASTNode* node = parser.parseStatement();

            if (!node) {
                return true; // abort
            }

            this->addChild(node);

            return false; // continue
        });

        parser.context()->popScope();

        return true;
    }

    std::string FunctionDefinitionNode::fullName() const {
        std::stringstream s;

        for (const std::string& part : namespaceComponents) {
            s << part << "_3_";
        }

        if (_methodOnType.kind() != NewDataType::Kind::Undefined) {
            s << _methodOnType.name() << "_3_";
        }

        s << this->name();

        return s.str();
    }
}
