#include "FunctionDefinitionNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/Lexer/Lexer.h"
#include "compiler/constructs/Scope.h"
#include "compiler/constructs/QualifiedName.h"
#include "compiler/AST/Atomics/AtomicNode.h"

#include "compiler/Messages/DuplicateFunctionMessage.h"

#include <assert.h>
#include <sstream>

namespace Three {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordDef));

        FunctionDefinitionNode* node = new FunctionDefinitionNode();

        node->setVisibility(parser.context()->visibility());

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: Expected function name or method type");
        }

        // we have a method
        if (parser.isAtType()) {
            node->_methodOnType = parser.parseType();

            assert(parser.helper()->nextIf(Token::Type::OperatorDot));
        }

        // sanity check the type we are at, and make sure we can actually
        // use the name selected
        assert(parser.helper()->peek().type() == Token::Type::Identifier);
        if (!parser.isAtIdentifierAvailableForDefinition()) {
            assert(0 && "Message: function name isn't avaiable for definition");
        }

        // We have to store this, because we loose the scope when we pass over the function body.
        node->_scopedNamespace = parser.context()->scope()->fullNamespace();

        node->_name = QualifiedName(parser.helper()->nextStr());

        // If this is a method, insert the Type name into to functions full name.  This
        // is a little tricky though, as the two could share a namespace, or even
        // some of a namespace.
        // Type.method => Type,method
        // Name::Spaced::Type.method => Name,Spaced,Type,method
        // Name::Spaced::Type.Name::Spaced::method => Name,Spaced,Type,method
        if (node->_methodOnType.kind() != DataType::Kind::Undefined) {
            // We have to prepend *both* the type name and its namespace. This
            // is kind of a messy way to do that.
            QualifiedName typeNameAsNamespace;

            typeNameAsNamespace.prependName(node->_methodOnType.qualifiedName());
            typeNameAsNamespace.appendComponent(node->_methodOnType.qualifiedName().name());

            node->_name.prependName(typeNameAsNamespace);
        }

        // now, put the entire name in a namespace. But, not if the type is in the
        // exact same space.
        if (!node->_methodOnType.qualifiedName().sameNamespaceAs(node->_scopedNamespace)) {
            node->_name.prependName(node->_scopedNamespace);
        }

        // Might be generic.
        // This part is a little weird, because we have to parse the generic
        // params before we have the function type.
        DataType genericType;
        if (parser.helper()->peek().type() == Token::Type::OperatorLessThan) {
            if (!parser.parseGenericParameters(genericType)) {
                assert(0 && "Message: Unable to parse generic parameters");
            }
        }

        // parse signature
        node->_functionType = parser.parseFunctionSignatureType();
        node->_functionType.setLabel(node->fullName());

        if (genericType.isGeneric()) {
            // and now, if we really did parse generic params, we need to "merge"
            // them into the function type.
            for (uint32_t i = 0; i < genericType.genericParameterCount(); ++i) {
                node->_functionType.addGenericParameter(genericType.genericParameterAtIndex(i));
            }
        }

        // insert the self parameter, if needed
        if (node->isMethod()) {
            DataType selfPtr = DataType::wrapInPointer(node->_methodOnType);
            selfPtr.setLabel("self");

            node->_functionType.parameters.insert(node->_functionType.parameters.cbegin(), selfPtr);
        }

        if (!parser.context()->defineFunctionForName(node->_functionType, node->fullName())) {
            parser.context()->addMessage(new DuplicateFunctionMessage(node->fullName()));

            delete node;

            return nullptr;
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

        bool result = bufferOpenToCloseToken(parser, stream);

        parser.helper()->lexer()->setFilterWhitespace(true);

        return result;
    }

    bool FunctionDefinitionNode::bufferOpenToCloseToken(Parser& parser, std::stringstream& stream) {
        int nestedEnds = 0;
        bool onlyWhitespaceSoFar = true;

        // guard against infinite loops
        for (int i = 0; i < 100000; ++i) {
            Token t = parser.helper()->peek();

            switch (t.type()) {
                case Token::Type::KeywordDef:
                case Token::Type::KeywordNamespace:
                case Token::Type::KeywordImport:
                case Token::Type::KeywordInclude:
                case Token::Type::Undefined:
                case Token::Type::EndOfInput:
                    // all invalid inside a function
                    return false;
                case Token::Type::Newline:
                    // reset our check for non-whitespace tokens
                    onlyWhitespaceSoFar = true;
                    break;
                case Token::Type::Whitespace:
                    // skip this, because we have to maintain
                    // our state about whitespace
                    break;
                case Token::Type::KeywordEnd:
                    if (nestedEnds == 0) {
                        return true;
                    }

                    assert(nestedEnds > 0);
                    nestedEnds--;

                    break;
                default:
                    // What's the deal with this whitespace stuff? Consider the following:
                    // "x = 1 if true\n"
                    // "if abc\n"
                    // The first one does not need a terminating "end", but the second does.
                    if (!onlyWhitespaceSoFar) {
                        break;
                    }

                    onlyWhitespaceSoFar = false;

                    // if this token isn't closed by an end, we have nothing to worry about
                    if (!t.mightBeClosedByEndKeyword()) {
                        break;
                    }

                    // The "atomic" keyword is another language construct that might or
                    // might not be terminated by an end.
                    if (AtomicNode::isAtAtomicExpression(parser)) {
                        break;
                    }

                    // if we got here, we have encounterd a token that needs a matching
                    // end
                    nestedEnds++;

                    break;
            }

            stream << parser.helper()->nextStr();
        }

        assert(0 && "Exceeded maximum number of tokens inside a function");

        return false;
    }

    std::string FunctionDefinitionNode::nodeName() const {
        return "Function Definition";
    }

    std::string FunctionDefinitionNode::name() const {
        return _name.name();
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << "Function: " << this->fullName();

        return s.str();
    }

    void FunctionDefinitionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    DataType FunctionDefinitionNode::functionType() const {
        return _functionType;
    }

    DataType FunctionDefinitionNode::methodOnType() const {
        return _methodOnType;
    }
    
    bool FunctionDefinitionNode::isMethod() const {
        return _methodOnType.kind() != DataType::Kind::Undefined;
    }

    std::stringstream* FunctionDefinitionNode::bodyStream() {
        return &_bodyString;
    }

    void FunctionDefinitionNode::eachNamedReturn(std::function<void (const DataType&)> block) const {
        // tuples have to be treated special here
        if (_functionType.returnType().kind() == DataType::Kind::Tuple) {
            _functionType.returnType().eachSubtypeWithLast([&] (const DataType& type, bool last) {
                if (type.label().size() > 0) {
                    block(DataType::mutableVersion(type));
                }
            });

            return;
        }

        // just a simple return type
        if (_functionType.returnType().label().size() > 0) {
            block(DataType::mutableVersion(_functionType.returnType()));
        }
    }

    void FunctionDefinitionNode::defineVariableForReturnType(Scope* scope, const DataType& type) const {
        assert(type.label().size() != 0);

        scope->defineVariableTypeForName(type, type.label());
    }

    void FunctionDefinitionNode::defineParameterVariablesInScope(Scope* scope) const {
        // create variables for all arguments in this scope, including the special self
        // variable if this is a method
        _functionType.eachParameterWithLast([&] (const DataType& type, bool last) {
            scope->defineVariableTypeForName(type, type.label());
        });

        // now, create variables for each named return
        this->eachNamedReturn([&] (const DataType& type) {
            this->defineVariableForReturnType(scope, type);
        });
    }

    bool FunctionDefinitionNode::parseBody(Parser& parser) {
        parser.context()->pushScope();
        parser.context()->scope()->setCurrentFunctionReturnType(_functionType.returnType());

        // create a scope that reflects the state at the time of parse, including
        // the namespace, if any
        parser.context()->scope()->setNamespace(_scopedNamespace);
        parser.context()->scope()->setScopedBasename(this->name());

        // Ok, this is pretty wack. We've already done this, but the scope might have been destroyed, so
        // we cannot reference ourselves. If the scope still exists, this will fail, which is ok.
        parser.context()->scope()->defineVariableTypeForName(_functionType, this->fullName());

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
        if (_methodOnType.kind() != DataType::Kind::Undefined) {
        }

        return _name.to_s();
    }
}
