#include "Parser.h"
#include "ParseHelper.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/Scope.h"
#include "compiler/Lexer/Lexer.h"
#include "compiler/Preprocessing/Preprocessor.h"

#include "compiler/AST.h"

#include "compiler/Messages/EmptyInputMessage.h"
#include "compiler/Messages/UnparsableMessage.h"
#include "compiler/Messages/UnableToCompleteParseMessage.h"
#include "compiler/Messages/ExpectedIdentifierMessage.h"
#include "compiler/Messages/ExpectedTypeIdentifierPairMessage.h"

#include <fstream>

namespace Three {
    bool Parser::parse(const char* inputPath, ParseContext* context) {
        std::ifstream file(inputPath);

        // pre-process
        std::string preprocessedString = Three::Preprocessor::process(&file);

        std::istringstream stream(preprocessedString);

        // create a lexer from the pre-processed stream
        Three::Lexer lexer(&stream);

        Three::Parser parser;

        if (!parser.parse(&lexer, context)) {
            return false;
        }

        if (context->displayMessages()) {
            return false;
        }

        context->clearMessages();

        if (!parser.parseFunctionBodies(context)) {
            return false;
        }

        if (context->displayMessages()) {
            return false;
        }

        return true;
    }

    Parser::Parser() {
        _characterEncodingMap["ascii"]   = DataType::CharacterEncoding::ASCII;
        _characterEncodingMap["utf8"]    = DataType::CharacterEncoding::UTF8;
        _characterEncodingMap["utf16"]   = DataType::CharacterEncoding::UTF16;
        _characterEncodingMap["utf16le"] = DataType::CharacterEncoding::UTF16LE;
        _characterEncodingMap["utf16be"] = DataType::CharacterEncoding::UTF16BE;
        _characterEncodingMap["utf32"]   = DataType::CharacterEncoding::UTF32;
        _characterEncodingMap["utf32le"] = DataType::CharacterEncoding::UTF32LE;
        _characterEncodingMap["utf32be"] = DataType::CharacterEncoding::UTF32BE;
    }

    Parser::~Parser() {
    }

    ParseContext* Parser::context() const {
        return _context;
    }

    ParseHelper* Parser::helper() const {
        return _helper;
    }

    bool Parser::verbose() const {
        return false;
    }

    bool Parser::parse(Lexer* lexer, ParseContext* context) {
        assert(lexer && "Lexer cannot be null");
        assert(context && "ParseContext cannot be null");

        _context = context;

        ParseHelper helper(lexer);
        _helper = &helper;

        // do stuff here
        this->startParse();

        _helper = nullptr;

        if (context->rootNode()->childCount() == 0) {
            context->addMessage(new EmptyInputMessage());
        }

        _context = nullptr;

        return true;
    }

    void Parser::startParse() {
        _helper->parseUntil(false, [&] (const Token& token) {
            bool success = false;

            ASTNode* node = this->parseTopLevelNode(&success);

            // there's no need to check success here
            return !_context->rootNode()->addChild(node);
        });

        // if there are tokens left here, its an error
        if (!_helper->lexer()->atEnd()) {
            _context->addMessage(new UnableToCompleteParseMessage());
        }
    }

    bool Parser::parseFunctionBodies(ParseContext* context) {
        _context = context;

        for (FunctionDefinitionNode* node : _functions) {
            this->parseFunctionBody(node);
        }

        _context = nullptr;

        return true;
    }

    void Parser::parseFunctionBody(FunctionDefinitionNode* func) {
        assert(func);

        Lexer lexer(func->bodyStream());

        ParseHelper helper(&lexer);
        _helper = &helper;

        if (!func->parseBody(*this)) {
            assert(false && "Message: unable to parse function body");
        }

        _helper = nullptr;
    }

    ASTNode* Parser::parseTopLevelNode(bool* success) {
        assert(success);
        *success = false;

        // first thing is first, advance past newlines
        _helper->parseNewlines();

        if (this->verbose()) {
            std::cout << "Parser: top level '" << this->helper()->peek().str() << "'" << std::endl;
        }

        ASTNode* node = nullptr;

        // easy cases
        switch (this->helper()->peek().type()) {
            case Token::Type::Undefined:
            case Token::Type::EndOfInput:
                // we're done here
                *success = true;
                return nullptr;
            case Token::Type::AnnotationBrief:
            case Token::Type::AnnotationSummary:
            case Token::Type::AnnotationParam:
            case Token::Type::AnnotationReturn:
            case Token::Type::AnnotationNoreturn:
                node = AnnotationNode::parse(*this);
                break;
            case Token::Type::KeywordDef:
                node = FunctionDefinitionNode::parse(*this);
                break;
            case Token::Type::KeywordImport:
                node = ImportNode::parse(*this);
                break;
            case Token::Type::KeywordInclude:
                node = IncludeNode::parse(*this);
                break;
            case Token::Type::KeywordStructure:
            case Token::Type::KeywordEnumeration:
            case Token::Type::KeywordUnion:
                node = CompositeTypeDefinitionNode::parse(*this);
                break;
            case Token::Type::KeywordLinkage:
            case Token::Type::KeywordModule:
                assert(0 && "Unimplemented: directives");
                break;
            case Token::Type::KeywordPublic:
            case Token::Type::KeywordPrivate:
                node = VisibilityNode::parse(*this);
                break;
            case Token::Type::KeywordNamespace:
                node = NamespaceNode::parse(*this);
                break;
            case Token::Type::KeywordDebug:
                node = DebugNode::parse(*this);
                break;
            case Token::Type::KeywordEnd:
                // Tokens that are invalid in a top-level context should go here.
                // TODO: There are lots more.
                return nullptr;
            default:
                // last possibility, global variable definition
                // TODO: similar to the comment above, there are only some
                // tokens that could possibly allow for a global.
                node = VariableDeclarationNode::parseGlobal(*this);
                break;
        }

        if (node) {
            *success = true;
        }

        return node;
    }

    ASTNode* Parser::parseStatement() {
        ASTNode* node = nullptr;

        // advance past newlines here
        _helper->parseNewlines();

        if (this->verbose()) {
            std::cout << "Parser: statement '" << this->helper()->peek().str() << "'" << std::endl;
        }

        // Checking for tailing ifs is a little tricky here.  Certain things
        // can have them, certain ones cannot.
        switch (_helper->peek().type()) {
            case Token::Type::KeywordReturn:
                node = IfNode::parseTailing(*this, ReturnNode::parse(*this));
                break;
            case Token::Type::KeywordIf:
                node = IfNode::parse(*this);
                break;
            case Token::Type::KeywordLoop:
                node = LoopNode::parse(*this);
                break;
            case Token::Type::KeywordFor:
                node = ForNode::parse(*this);
                break;
            case Token::Type::KeywordBreak:
                node = IfNode::parseTailing(*this, BreakNode::parse(*this));
                break;
            case Token::Type::KeywordContinue:
                node = IfNode::parseTailing(*this, ContinueNode::parse(*this));
                break;
            case Token::Type::KeywordSwitch:
                node = SwitchNode::parse(*this);
                break;
            case Token::Type::KeywordAtomic:
                node = AtomicNode::parse(*this);
                break;
            case Token::Type::KeywordAbort:
                node = IfNode::parseTailing(*this, AbortStatementNode::parse(*this));
                break;
            case Token::Type::KeywordBarrier:
                node = IfNode::parseTailing(*this, BarrierNode::parse(*this));
                break;
            case Token::Type::KeywordDebug:
                node = DebugNode::parse(*this);
                break;
            default:
                if (this->isAtType()) {
                    node = VariableDeclarationNode::parse(*this);
                    node->setStatement(true);
                } else {
                    node = this->parseExpressionStatement();
                }
                break;

        }

        // parse newline(s), which should always appear at the end of statement, unless
        // we've completed the input
        if (!_helper->lexer()->atEnd() && !_helper->parseNewlines()) {
            //assert(0 && "Message: statement should always terminate with a newline");
        }

        return node;
    }

    ASTNode* Parser::parseExpressionStatement() {
        ASTNode* node = this->parseExpression();

        if (!node) {
            return nullptr;
        }

        if (_helper->peek().type() == Token::Type::PunctuationComma) {
            // we have a destructuring assignment
            node = DestructuredAssignmentOperatorNode::parse(*this, node);
        }

        if (!node) {
            return nullptr;
        }

        node->setStatement(true);

        // return IfNode::parseTailing(this->parseExpression());
        return node;
    }

    ASTNode* Parser::parseExpressionWithTuples(const DataType* expectedType) {
        ASTNode* node = this->parseExpression();

        if (!node) {
            return nullptr;
        }

        if (_helper->peek().type() == Token::Type::PunctuationComma) {
            return TupleNode::parse(*this, node, true, expectedType);
        }

        return node;
    }

    ASTNode* Parser::parseExpression(uint32_t precedence) {
        // Parsing expressions is very complex, and depends on correctly setting
        // the precedence and associativity of operator tokens.
        // The algorithm used is called Precedence Climbing.
        assert(precedence > Token::NonPrecedence);

        if (this->verbose()) {
            std::cout << "Parser: parseExpression (" << precedence << ")" << std::endl;
        }

        ASTNode* left = this->parseExpressionElement();

        while (true) {
            Token t = _helper->peek();

            // if we've gotten to the end of a statement, or do not have an operator
            if (t.isStatementEnding() || !t.isOperator()) {
                break;
            }

            // ok we have a binary operator token
            uint32_t nextPrecedence = t.precedence();
            assert(nextPrecedence != Token::NonPrecedence);

            if (nextPrecedence < precedence) {
                if (this->verbose()) {
                    std::cout << "Parser: parseExpression lower " << t.str() << " (" << nextPrecedence << ")" << std::endl;
                }

                break;
            }

            if (t.isLeftAssociative()) {
                nextPrecedence = nextPrecedence + 1;
            }

            if (this->verbose()) {
                std::cout << "Parser: parseExpression " << t.str() << " : " << nextPrecedence << " compare " << precedence << std::endl;
            }

            left = OperatorNode::parse(*this, left, nextPrecedence);
        }

        return left;
    }

    ASTNode* Parser::parseExpressionElement() {
        ASTNode* node = this->parseExpressionElementWithoutTailingOperators();

        if (!node) {
            return nullptr;
        }

        return OperatorNode::parseTailing(*this, node);
    }

    ASTNode* Parser::parseExpressionElementWithoutTailingOperators() {
        ASTNode* node = nullptr;

        if (this->verbose()) {
            std::cout << "Parser: parseSecondaryExpression: '" << _helper->peek().str() << "'" << std::endl;
        }

        // try a unary operator first
        if (_helper->peek().isUnaryOperator()) {
            if (this->verbose()) {
                std::cout << "Parser: unary in parseSecondaryExpression: '" << _helper->peek().str() << "'" << std::endl;
            }

            return OperatorNode::parseUnary(*this);
        }

        switch (_helper->peek().type()) {
            case Token::Type::Identifier:
                return this->parseExpressionIdentifier();
            case Token::Type::KeywordClosure:
                return ClosureNode::parse(*this);
            case Token::Type::LiteralString:
                return StringLiteralNode::parse(*this);
            case Token::Type::LiteralInteger:
            case Token::Type::LiteralHex:
            case Token::Type::LiteralBinary:
                return IntegerLiteralNode::parse(*this);
            case Token::Type::LiteralReal:
            case Token::Type::LiteralScientificNotation:
            case Token::Type::LiteralCharacter:
            case Token::Type::LiteralTrue:
            case Token::Type::LiteralFalse:
                return LiteralNode::parse(*this);
            case Token::Type::PunctuationOpenParen: {
                ASTNode* node = nullptr;

                this->parseParentheses([&] () {
                    node = this->parseExpression();
                });

                return node;
            }
            case Token::Type::LiteralNull:
                return NullLiteralNode::parse(*this);
            case Token::Type::KeywordAtomic:
                return AtomicExpressionNode::parse(*this);
            case Token::Type::MetafunctionSizeOf:
            case Token::Type::MetafunctionCast:
            case Token::Type::MetafunctionAlignof:
            case Token::Type::MetafunctionOffsetof:
            case Token::Type::MetafunctionTypeof:
            case Token::Type::MetafunctionNameof:
            case Token::Type::MetafunctionCardinalityof:
            case Token::Type::MetafunctionNextarg:
                return MetafunctionNode::parse(*this);
            default:
                break;
        }

        return nullptr;
    }

    ASTNode* Parser::parseExpressionIdentifier() {
        assert(_helper->peek().type() == Token::Type::Identifier);

        QualifiedName name = this->parseMultiPartIdentifierComponents();
    
        assert(name.components.size() > 0);

        // at this point, we could have:
        // - function
        // - C function
        // - C constant
        // - C macro
        // - global variable
        // - local variable

        // TODO: I think at this point we really need some kind of symbol table. Dealing
        // with C symbols is tough, and that could reduce the number of heurestics needed.
        Variable* variable = _context->variableForName(name.to_s());
        if (variable) {
            if (variable->type.kind() == DataType::Kind::CUnspecifiedMacro) {
                return new CMacroNode(name.to_s());
            }
        }

        ASTNode* node = VariableNode::parse(*this, name);

        return node;
    }

    bool Parser::parseParentheses(std::function<void (void)> func) {
        if (!_helper->nextIf(Token::Type::PunctuationOpenParen)) {
            return false;
        }

        func();

        return _helper->nextIf(Token::Type::PunctuationCloseParen);
    }

    std::string Parser::parseMultiPartIdentifier() {
        return this->parseMultiPartIdentifierComponents().to_s();
    }

    QualifiedName Parser::parseMultiPartIdentifierComponents() {
        QualifiedName name;

        for (;;) {
            // have to start with an identifier
            if (_helper->peek().type() != Token::Type::Identifier) {
                assert(0 && "Message: Invalid multi-part identifier");
            }

            name.appendComponent(_helper->nextStr());

            // need two successive colons to continue
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                break;
            }

            if (_helper->peek(2).type() != Token::Type::PunctuationColon) {
                break;
            }

            // check for and handle the case of empty specifiers
            if (_helper->peek(3).type() != Token::Type::Identifier) {
                break;
            }

            _helper->next();
            _helper->next();
        }

        return name;
    }

    bool Parser::parseBodyWithElse(const std::string& label, ASTNode** elseNode, std::function<void (void)> func) {
        assert(elseNode);
        *elseNode = nullptr;

        _helper->parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                    return true;
                case Token::Type::KeywordElse:
                    *elseNode = ElseNode::parse(*this);
                    return true;
                default:
                    func();
                    return false;
                }
        });

        if (!_helper->nextIf(Token::Type::KeywordEnd)) {
            assert(0 && "Message: statement body must be terminated with an end");
            return false;
        }

        return true;
    }

    bool Parser::parseBodyWithElse(ASTNode* node, ASTNode** elseNode) {
        return this->parseBodyWithElse(node->nodeName(), elseNode, [&] () {
            node->addChild(this->parseStatement());
        });
    }

    uint32_t Parser::peekDepthIfAtType(uint32_t peekDepth) {
        if (this->peekType(&peekDepth)) {
            return peekDepth;
        }

        return 0;
    }

    bool Parser::isAtType() {
        return this->peekDepthIfAtType() > 0;
    }

    bool Parser::peekType(unsigned int* peekDepth) {
        // move past any type prefixes
        this->peekTypePrefixes(peekDepth);

        if (this->peekNonFunctionType(peekDepth)) {
            this->peekTypePostfixes(peekDepth);
            return true;
        }

        if (this->peekFunctionType(peekDepth)) {
            this->peekTypePostfixes(peekDepth);
            return true;
        }

        return false;
    }

    bool Parser::peekTypePrefixes(unsigned int* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        // Pointers, arrays, annotations could be intermixed.
        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::OperatorStar:
                    *peekDepth += 1;
                    this->peekTypePostfixes(peekDepth);
                    continue;
                case Token::Type::PunctuationOpenBracket:
                    *peekDepth += 1;

                    if (_helper->peek(*peekDepth).type() != Token::Type::LiteralInteger) {
                        return false;
                    }
                    *peekDepth += 1;

                    if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationCloseBracket) {
                        return false;
                    }
                    *peekDepth += 1;

                    continue;
                case Token::Type::AnnotationAccess:
                case Token::Type::AnnotationVolatile:
                case Token::Type::AnnotationAlias:
                case Token::Type::AnnotationConst:
                case Token::Type::AnnotationRestrict:
                    this->peekTypeAnnotations(peekDepth);
                    continue;
                default:
                    return false;
            }

            break; // kill the loop
        }

        // return true if we've advanced at all
        return *peekDepth > originalDepth;
    }

    bool Parser::peekTypePostfixes(unsigned int* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::OperatorNot:
                case Token::Type::OperatorQuestionMark:
                    *peekDepth += 1;
                    continue;
                default:
                    break;
            }

            break; // kill the loop
        }

        // return true if we've advanced at all
        return *peekDepth > originalDepth;
    }

    bool Parser::peekTypeAnnotations(unsigned int* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::AnnotationConst:
                case Token::Type::AnnotationRestrict:
                    // these two area easy, because they take no arguments
                    *peekDepth += 1;
                    continue;
                case Token::Type::AnnotationVolatile:
                    // volatile can appear with no argument
                    if (_helper->peek(*peekDepth+1).type() != Token::Type::PunctuationOpenParen) {
                        *peekDepth += 1;
                        continue;
                    }
                    // intentional fall-through
                case Token::Type::AnnotationAccess:
                case Token::Type::AnnotationAlias:
                    if (_helper->peek(*peekDepth+1).type() != Token::Type::PunctuationOpenParen) {
                        return false;
                    }

                    // TODO: this isn't particularly strict syntax checking

                    if (_helper->peek(*peekDepth+3).type() != Token::Type::PunctuationCloseParen) {
                        return false;
                    }

                    *peekDepth += 4;
                    continue;
                default:
                    return false;
            }

            break;
        }

        return *peekDepth > originalDepth;
    }

    bool Parser::peekFunctionType(unsigned int* peekDepth) {
        Token::Type closingPunctuation;

        // "{} Identifier"
        // "() Identifier"
        // "{; Type} Identifier"
        // "(; Type) Identifier"

        // first, figure out if we have a closure or function, and what
        // the ending punctuation is
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                closingPunctuation = _helper->peek(*peekDepth).closingCounterpart();
                break;
            default:
                return false;
        }

        *peekDepth += 1;

        // We need to advance past the parameters and returns, if any. Just for safety,
        // add a sanity check to prevent loops.
        for (int i = 0;; ++i) {
            assert(i < 100 && "Trapped potential infinite loop checking for function type");

            Token::Type type = _helper->peek(*peekDepth).type();

            if (type == closingPunctuation) {
                *peekDepth += 1;
                return true;
            }

            if (type == Token::Type::PunctuationSemicolon) {
                // advance past this, but we now are definitely expecting a type
                *peekDepth += 1;
            }

            // there must be a type here
            if (!this->peekType(peekDepth)) {
                return false;
            }

            // advance past the comma, and continue
            if (_helper->peek(*peekDepth).type() == Token::Type::PunctuationComma) {
                *peekDepth += 1;

                // there must be a type here too
                if (!this->peekType(peekDepth)) {
                    return false;
                }
            }
        }

        return false;
    }

    bool Parser::peekNonFunctionType(unsigned int* peekDepth) {
        if (_helper->peek(*peekDepth).type() == Token::Type::KeywordVararg) {
            *peekDepth += 1;
            return true;
        }

        // We have to lead with an identifier. We also need to do a little
        // dance with the peekDepth, because we could be at an identifier
        // that isn't a type, and that will change peekDepth;
        QualifiedName name;
        unsigned int depth = *peekDepth;
        if (!this->peekScopedIdentifier(&depth, name)) {
            return false;
        }

        if (!_context->definesTypeWithName(name)) {
            return false;
        }

        // assign the depth value, now that we have a confirmed type
        *peekDepth = depth;

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }
        
        *peekDepth += 1;

        // First specifier, could be an identifier, a number, or another colon
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::Identifier:
            case Token::Type::LiteralInteger:
                *peekDepth += 1;
                break;
            case Token::Type::PunctuationColon:
                break;
            default:
                return false; // not a type
        }

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }

        *peekDepth += 1;

        // Second specifier, could be a number or another colon
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::LiteralInteger:
                *peekDepth += 1;
                break;
            case Token::Type::PunctuationColon:
                break;
            default:
                return false; // not a type
        }

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }

        *peekDepth += 1;

        // Third specifier - must be a number
        if (_helper->peek(*peekDepth).type() != Token::Type::LiteralInteger) {
            return false;
        }

        *peekDepth += 1;

        return true;
    }

    bool Parser::peekScopedIdentifier(unsigned int* peekDepth, QualifiedName& peekedName) {
        assert(peekDepth);

        for (;;) {
            // We have to lead with an identifier
            if (_helper->peek(*peekDepth).type() != Token::Type::Identifier) {
                return false;
            }

            peekedName.appendComponent(_helper->peek(*peekDepth).str());
            *peekDepth += 1;

            // check for "::"
            if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
                return true;
            }

            if (_helper->peek(*peekDepth + 1).type() != Token::Type::PunctuationColon) {
                return true;
            }

            // Check for the empty specifier case of "Int::4"
            if (_helper->peek(*peekDepth + 2).type() != Token::Type::Identifier) {
                return true;
            }

            // move past the "::", and check for the next identifier part
            *peekDepth += 2;
        }

        return false;
    }

    DataType Parser::parseType(bool genericParam) {
        if (this->verbose()) {
            std::cout << "Parser: parseType '" << this->helper()->peek().str() << "'" << std::endl;
        }
        return this->parseAndApplyTypeAnnotations(genericParam);
    }

    DataType Parser::parseAndApplyTypeAnnotations(bool genericParam) {
        bool foundConst = false;
        bool foundRestrict = false;
        bool foundVolatile = false;
        bool foundAccess = false;
        bool foundAlias = false;

        DataType::Access volatility = DataType::Access::ReadWrite;
        DataType::Access access = DataType::Access::None;

        for (;;) {
            switch (_helper->peek().type()) {
                case Token::Type::AnnotationConst:
                    if (foundConst) {
                        assert(0 && "Message: @const cannot be applied more than once");
                    }

                    _helper->next();
                    foundConst = true;
                    continue;
                case Token::Type::AnnotationRestrict:
                    if (foundRestrict) {
                        assert(0 && "Message: @restrict cannot be applied more than once");
                    }

                    _helper->next();
                    foundRestrict = true;
                    continue;
                case Token::Type::AnnotationVolatile:
                    if (foundVolatile) {
                        assert(0 && "Message: @volatile cannot be applied more than once");
                    }

                    _helper->next();
                    foundVolatile = true;

                    // there might not be parens here, and that is ok
                    this->parseParentheses([&] () {
                        volatility = this->parseAnnotationAccess();
                    });

                    continue;
                case Token::Type::AnnotationAccess:
                    if (foundAccess) {
                        assert(0 && "Message: @access cannot be applied more than once");
                    }

                    _helper->next();
                    foundAccess = true;

                    this->parseParentheses([&] () {
                        access = this->parseAnnotationAccess();
                    });

                    continue;
                case Token::Type::AnnotationAlias:
                    if (foundAlias) {
                        assert(0 && "Message: @alias cannot be applied more than once");
                    }

                    _helper->next();
                    foundAlias = true;

                    this->parseParentheses([&] () {
                        if (!_helper->nextIf("none")) {
                            assert(0 && "Message: @alias supports only none");
                        }
                    });

                    continue;
                default:
                    break;
            }

            break; // end the loop
        }

        // now that we've parsed all the annotations, we can proceed to the type itself
        DataType type = this->parseTypeWithoutAnnotations(genericParam);
        type = this->parseTypePostfixes(type);

        if (foundConst && foundAccess) {
            assert(0 && "Message: @const and @access cannot be applied at the same time");
        }

        if (foundAlias && foundRestrict) {
            assert(0 && "Message: @alias and @restrict cannot be applied at the same time");
        }

        if (foundConst) {
            type.setAccess(DataType::Access::Read);
        }

        if (foundRestrict) {
            if (type.kind() != DataType::Kind::Pointer) {
                assert(0 && "Message: @restrict applies to Pointer types only");
            }

            type.setPotentiallyAliased(true);
        }

        if (foundVolatile) {
            type.setVolatility(volatility);
        }

        if (foundAccess) {
            type.setAccess(access);
        }

        if (foundAlias) {
            type.setPotentiallyAliased(true);
        }

        return type;
    }

    DataType Parser::parseTypeWithoutAnnotations(bool genericParam) {
        // Non-recurisve, so should be a type name
        if (this->verbose()) {
            std::cout << "Parser: parseTypeWithoutAnnotations '" << this->helper()->peek().str() << "'" << std::endl;
        }

        // Handle recursive types (pointer and array)
        switch (_helper->peek().type()) {
            case Token::Type::OperatorStar:
                return this->parsePointerType(genericParam);
            case Token::Type::PunctuationOpenBracket:
                return this->parseArrayType(genericParam);
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                assert(!genericParam && "Message: function types be a generic parameter");
                return this->parseFunctionType();
            case Token::Type::KeywordVararg:
                assert(!genericParam && "Message: vararg cannot be a generic parameter");
                _helper->next();
                return DataType(DataType::Kind::Vararg);
            default:
                break;
        }

        if (this->helper()->peek().type() != Token::Type::Identifier) {
            std::cout << this->helper()->peek().str() << std::endl;
            assert(0 && "Message: type should always be an identifier");
        }

        QualifiedName name = this->parseMultiPartIdentifierComponents();

        DataType type;
        if (genericParam) {
            type = DataType(DataType::Kind::Generic);
            if (!_context->identifierAvailableForDefinition(name.to_s())) {
                assert(0 && "Message: generic parameter identifier already defined in this scope");
            }

            type.setName(name.to_s());
        } else {
            type = _context->dataTypeForName(name.to_s());
        }

        if (type.kind() == DataType::Kind::Undefined) {
            std::cout << name.to_s() << std::endl;
            assert(0 && "Message: failed to parse type");
        }

        // handle character specifiers
        if (type.kind() == DataType::Kind::Character) {
            if (_helper->nextIf(Token::Type::PunctuationColon)) {
                type.setCharacterEncoding(this->parseCharacterEncodingSpecifier());
            }

            return type;
        }

        this->parseTypeSpecifiers(type);

        return type;
    }

    void Parser::parseTypeSpecifiers(DataType& type) {
        // width specifier (or, colon)
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                type.setWidthSpecifier(this->parseIntegerSpecifier());
            }
        }

        // alignment (or, colon)
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                type.setAlignmentSpecifier(this->parseIntegerSpecifier());
            }
        }

        // vector
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            type.setVectorSizeSpecifier(this->parseIntegerSpecifier());
        }
    }

    DataType Parser::parseTypePostfixes(const DataType& type) {
        DataType newType(type);

        if (_helper->nextIf(Token::Type::OperatorNot)) {
            newType.setAccess(DataType::Access::ReadWrite);
        }

        if (_helper->nextIf(Token::Type::OperatorQuestionMark)) {
            switch (type.kind()) {
                case DataType::Kind::Pointer:
                    newType.setKind(DataType::Kind::NullablePointer);
                    break;
                case DataType::Kind::GenericPointer:
                    newType.setKind(DataType::Kind::GenericNullablePointer);
                    break;
                case DataType::Kind::Generic:
                    newType = DataType::wrapInType(DataType::Kind::GenericNullablePointer, newType);
                    break;
                default:
                    newType = DataType::wrapInType(DataType::Kind::NullablePointer, newType);
                    break;
            }
        }

        return newType;
    }

    DataType Parser::parsePointerType(bool genericParam) {
        assert(_helper->next().type() == Token::Type::OperatorStar);

        DataType type = DataType(genericParam ? DataType::Kind::GenericPointer : DataType::Kind::Pointer);

        type = this->parseTypePostfixes(type);

        type.addSubtype(this->parseType(genericParam));

        return type;
    }

    DataType Parser::parseArrayType(bool genericParam) {
        assert(_helper->next().type() == Token::Type::PunctuationOpenBracket);

        DataType type = DataType(genericParam ? DataType::Kind::GenericArray : DataType::Kind::Array);

        if (_helper->peek().type() != Token::Type::PunctuationCloseBracket) {
            type.setArrayCount(this->parseIntegerSpecifier());
        }

        if (!_helper->nextIf(Token::Type::PunctuationCloseBracket)) {
            assert(0 && "Message: array type element count should be followed by a close bracket");
        }

        type.addSubtype(this->parseType(genericParam));

        return type;
    }

    DataType Parser::parseFunctionType(bool signature, std::vector<std::string>* references) {
        DataType type;
        Token::Type closingPunctuation = _helper->peek().closingCounterpart();

        switch (_helper->next().type()) {
            case Token::Type::PunctuationOpenParen:
                type = DataType(DataType::Kind::Function);
                break;
            case Token::Type::PunctuationOpenBrace:
                type = DataType(DataType::Kind::Closure);

                // make sure to define the environment pointer
                type.addParameter(DataType::wrapInPointer(DataType::Kind::Void));
                break;
            default:
                assert(0 && "Message: failed to parse a function type");
        }

        // check for ending punctuation
        if (_helper->nextIf(closingPunctuation)) {
            type.addReturn(DataType::Kind::Void);
            return type;
        }

        // now, we could have some parameters, so deal with those.
        // Type identifier,
        // Type identifier;
        // Type identifier)
        // identifier,
        // identifier;
        // identifier)
        for (int i = 0;; ++i) {
            bool mustParseType = false;

            assert(i < 100 && "Trapped potential infinite loop parsing function type parameters");

            if (!mustParseType) {
                if (_helper->nextIf(Token::Type::PunctuationSemicolon)) {
                    // move on to returns
                    break;
                }

                if (_helper->peek().type() == closingPunctuation) {
                    break;
                }
            }

            DataType paramType;

            if (signature) {
                std::string label = this->parseTypeIdentifierPair(paramType);
                if (label.size() == 0) {
                    return DataType();
                }

                paramType.setLabel(label);
            } else {
                paramType = this->parseType();
            }

            type.addParameter(paramType);

            // if we encounter a comma, make sure the next thing we do is try to parse a type
            mustParseType = _helper->nextIf(Token::Type::PunctuationComma);
        }

        // now, again, check for ending punctuation
        if (_helper->nextIf(closingPunctuation)) {
            type.addReturn(DataType::Kind::Void);
            return type;
        }

        // finally, we have returns
        DataType returnTuple = DataType(DataType::Kind::Tuple);

        for (int i = 0;; ++i) {
            bool mustParseType = false;

            assert(i < 100 && "Trapped potential infinite loop parsing function type returns");

            if (!mustParseType) {
                if (_helper->peek().type() == Token::Type::PunctuationSemicolon) {
                    break; // move on to references
                } else if (_helper->peek().type() == closingPunctuation) {
                    break;
                }
            }

            DataType returnType = this->parseType();

            if (signature && _helper->peek().type() == Token::Type::Identifier) {
                if (!this->isAtIdentifierAvailableForDefinition()) {
                    assert(0 && "Message: function return label invalid");
                }

                returnType.setLabel(_helper->nextStr());
            }

            returnTuple.addSubtype(returnType);

            // if we encounter a common, make sure the next thing we do is try to parse a type
            mustParseType = _helper->nextIf(Token::Type::PunctuationComma);
        }

        // fix up the return type
        switch (returnTuple.subtypeCount()) {
            case 0:
                type.addReturn(DataType::Kind::Void);
                break;
            case 1:
                // unwrap the tuple
                type.addReturn(returnTuple.subtypeAtIndex(0));
                break;
            default:
                type.addReturn(returnTuple);
                break;
        }

        // parse references
        if (_helper->nextIf(Token::Type::PunctuationSemicolon)) {
            assert(references && "Must have references at this point");
            for (int i = 0;; ++i) {
                if (_helper->peek().type() != Token::Type::Identifier) {
                    break;
                }

                references->push_back(_helper->nextStr());

                if (!_helper->nextIf(Token::Type::PunctuationComma)) {
                    break;
                }
            }
        }

        if (!_helper->nextIf(closingPunctuation)) {
            assert(0 && "Message: failed to find valid close punctuation for function type");
        }

        return type;
    }

    DataType Parser::parseFunctionSignatureType() {
        return this->parseFunctionType(true);
    }

    uint32_t Parser::parseIntegerSpecifier() {
        if (_helper->peek().type() != Token::Type::LiteralInteger) {
            assert(0 && "Message: width specifier being invalid");
            return 0;
        }

        return strtol(_helper->next().str().c_str(), NULL, 10);
    }

    std::string Parser::parseTypeIdentifierPair(DataType& type) {
        if (this->verbose()) {
            std::cout << "Parser: parseTypeIdentifierPair '" << this->helper()->peek().str() << "'" << std::endl;
        }

        // depth could be zero for untyped variables
        uint32_t depth = this->peekDepthIfAtType();
        if (depth > 0) {
            if (this->verbose()) {
                std::cout << "Parser: parseTypeIdentifierPair parsing type" << std::endl;
            }

            switch (_helper->peek(depth).type()) {
                case Token::Type::Identifier:
                case Token::Type::KeywordVararg:
                    type = this->parseType();
                    break;
                default:
                    assert(0 && "Message: should have been at type-identifier pair");
                    break;
            }
        }

        if (this->isAtIdentifierAvailableForDefinition()) {
            return _helper->nextStr();
        }

        _context->addMessage(new ExpectedTypeIdentifierPairMessage(""));

        return "";
    }

    DataType::Access Parser::parseAnnotationAccess() {
        if (_helper->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: annotation argument should be r/read, w/write, rw/readwrite, or n/none");
            return DataType::Access::None;
        }

        std::string str = _helper->next().str();
        if (str == "r" || str == "read") {
            return DataType::Access::Read;
        } else if (str == "w" || str == "write") {
            return DataType::Access::Write;
        } else if (str == "rw" || str == "readwrite") {
            return DataType::Access::ReadWrite;
        } else if (str == "n" || str == "none") {
            return DataType::Access::None;
        }

        assert(0 && "Message: annotation argument should be r/read, w/write, rw/readwrite, or n/none");
        return DataType::Access::None;
    }

    DataType::CharacterEncoding Parser::parseCharacterEncodingSpecifier() {
        assert(_helper->peek().type() == Token::Type::Identifier);

        auto it = _characterEncodingMap.find(_helper->next().str());

        if (it == _characterEncodingMap.cend()) {
            assert(0 && "Message about invalid character encoding");
            return DataType::CharacterEncoding::UTF8;
        }

        return it->second;
    }

    bool Parser::parseGenericParameters(DataType& type) {
        assert(_helper->nextIf(Token::Type::OperatorLessThan));

        type.addGenericParameter(Parser::parseType(true));

        return _helper->parseUntil(true, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::OperatorGreaterThan:
                    return true;
                case Token::Type::PunctuationComma:
                    _helper->next();
                    type.addGenericParameter(Parser::parseType(true));
                    return false;
                default:
                    assert(0 && "Message: Unexpected token while parsing generic parameters");
                    break;
            }

            return true;
        });
    }

    bool Parser::isAtIdentifierAvailableForDefinition() {
        if (_helper->peek().type() != Token::Type::Identifier) {
            _context->addMessage(new ExpectedIdentifierMessage(_helper->peek().str()));
            return false;
        }

        return _context->identifierAvailableForDefinition(_helper->peek().str());
    }

    void Parser::addFunctionForParsing(FunctionDefinitionNode* node) {
        _functions.push_back(node);
    }
}
