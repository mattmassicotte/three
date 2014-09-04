#include "Parser.h"
#include "ParseHelper.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/NewScope.h"
#include "compiler/Lexer/Lexer.h"
#include "compiler/Preprocessing/Preprocessor.h"

#include "compiler/AST.h"

#include "compiler/Messages/EmptyInputMessage.h"
#include "compiler/Messages/UnparsableMessage.h"
#include "compiler/Messages/UnableToCompleteParseMessage.h"

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
        _characterEncodingMap["ascii"]   = NewDataType::CharacterEncoding::ASCII;
        _characterEncodingMap["utf8"]    = NewDataType::CharacterEncoding::UTF8;
        _characterEncodingMap["utf16"]   = NewDataType::CharacterEncoding::UTF16;
        _characterEncodingMap["utf16le"] = NewDataType::CharacterEncoding::UTF16LE;
        _characterEncodingMap["utf16be"] = NewDataType::CharacterEncoding::UTF16BE;
        _characterEncodingMap["utf32"]   = NewDataType::CharacterEncoding::UTF32;
        _characterEncodingMap["utf32le"] = NewDataType::CharacterEncoding::UTF32LE;
        _characterEncodingMap["utf32be"] = NewDataType::CharacterEncoding::UTF32BE;
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
            ASTNode* node = this->parseTopLevelNode();

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

    ASTNode* Parser::parseTopLevelNode() {
        ASTNode* node = nullptr;

        if (this->verbose()) {
            std::cout << "Parser: top level '" << this->helper()->peek().str() << "'" << std::endl;
        }

        // first thing is first, advance past newlines
        if (_helper->peek().type() == Token::Type::Newline) {
            _helper->parseNewlines();
        }

        // easy cases
        switch (this->helper()->peek().type()) {
            case Token::Type::Undefined:
            case Token::Type::EndOfInput:
                // we're done here
                return nullptr;
            case Token::Type::AnnotationBrief:
            case Token::Type::AnnotationSummary:
            case Token::Type::AnnotationParam:
            case Token::Type::AnnotationReturn:
            case Token::Type::AnnotationNoreturn:
                return AnnotationNode::parse(*this);
            case Token::Type::KeywordDef:
                return FunctionDefinitionNode::parse(*this);
            case Token::Type::KeywordImport:
                return ImportNode::parse(*this);
            case Token::Type::KeywordInclude:
                return IncludeNode::parse(*this);
            case Token::Type::KeywordStructure:
            case Token::Type::KeywordEnumeration:
            case Token::Type::KeywordUnion:
                return CompositeTypeDefinitionNode::parse(*this);
            case Token::Type::KeywordLinkage:
            case Token::Type::KeywordModule:
                assert(0 && "Unimplemented: directives");
            case Token::Type::KeywordPublic:
            case Token::Type::KeywordPrivate:
                return VisibilityNode::parse(*this);
            case Token::Type::KeywordNamespace:
                return NamespaceNode::parse(*this);
            default:
                break;
        }

        // global variable definition
        node = VariableDeclarationNode::parseGlobal(*this);
        node->setStatement(true);

        if (!node) {
            _context->addMessage(new UnparsableMessage());
        }

        return node;
    }

    ASTNode* Parser::parseStatement() {
        ASTNode* node = nullptr;

        // advance past newlines here
        _helper->parseNewlines();

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
                node = AtomicStatementNode::parse(*this);
                break;
            case Token::Type::KeywordAbort:
                node = IfNode::parseTailing(*this, AbortStatementNode::parse(*this));
                break;
            case Token::Type::KeywordBarrier:
                node = IfNode::parseTailing(*this, BarrierNode::parse(*this));
                break;
            default:
                break;

        }

        if (!node && this->isAtType()) {
            node = VariableDeclarationNode::parse(*this);
            node->setStatement(true);
        }

        if (!node) {
            node = this->parseExpressionStatement();
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

    ASTNode* Parser::parseExpressionWithTuples(const NewDataType* expectedType) {
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
        return OperatorNode::parseTailing(*this, this->parseExpressionElementWithoutTailingOperators());
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

        std::string identifier = _helper->nextStr();

        // at this point, we could have:
        // - function
        // - C function
        // - C constant
        // - C macro
        // - global variable
        // - local variable

        // TODO: I think at this point we really need some kind of symbol table. Dealing
        // with C symbols is tough, and that could  reduce the number of heurestics needed.
        NewVariable* variable = _context->scope()->variableForName(identifier);
        if (variable) {
            if (variable->type.kind() == NewDataType::Kind::CUnspecifiedMacro) {
                return new CMacroNode(identifier);
            }
        }

        ASTNode* node = VariableNode::parse(*this, identifier);

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
        std::stringstream s;

        for (;;) {
            // have to start with an identifier
            if (_helper->peek().type() != Token::Type::Identifier) {
                assert(0 && "Message: Invalid multi-part identifier");
            }

            s << _helper->next().str();

            // need two successive colons to continue
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                break;
            }

            if (_helper->peek(2).type() != Token::Type::PunctuationColon) {
                break;
            }

            _helper->next();
            _helper->next();

            s << "::"; // this could be used to change the seperator
        }

        return s.str();
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

        // We have to lead with an identifier
        if (_helper->peek(*peekDepth).type() != Token::Type::Identifier) {
            return false;
        }

        // check to see if that identifier is a defined type
        if (!_context->definesTypeWithName(_helper->peek(*peekDepth).str())) {
            return false;
        }

        *peekDepth += 1;

        // At this point, we know we have an identifier, but we might have
        // specififers following it

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

    NewDataType Parser::parseType() {
        return this->parseAndApplyTypeAnnotations();
    }

    NewDataType Parser::parseAndApplyTypeAnnotations() {
        bool foundConst = false;
        bool foundRestrict = false;
        bool foundVolatile = false;
        bool foundAccess = false;
        bool foundAlias = false;

        NewDataType::Access volatility = NewDataType::Access::ReadWrite;
        NewDataType::Access access = NewDataType::Access::None;

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
        NewDataType type = this->parseTypeWithoutAnnotations();
        type = this->parseTypePostfixes(type, true);

        if (foundConst && foundAccess) {
            assert(0 && "Message: @const and @access cannot be applied at the same time");
        }

        if (foundAlias && foundRestrict) {
            assert(0 && "Message: @alias and @restrict cannot be applied at the same time");
        }

        if (foundConst) {
            type.setAccess(NewDataType::Access::Read);
        }

        if (foundRestrict) {
            if (type.kind() != NewDataType::Kind::Pointer) {
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

    NewDataType Parser::parseTypeWithoutAnnotations() {
        // Handle recursive types (pointer and array)
        switch (_helper->peek().type()) {
            case Token::Type::OperatorStar:
                return this->parsePointerType();
            case Token::Type::PunctuationOpenBracket:
                return this->parseArrayType();
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                return this->parseFunctionType();
            default:
                break;
        }

        // Non-recurisve, so should be a type name
        NewDataType type = _context->dataTypeForName(_helper->peek().str());

        if (type.kind() == NewDataType::Kind::Undefined) {
            std::cout << _helper->peek().str() << std::endl;
            assert(0 && "Message: failed to parse type");
        }

        _helper->next();

        // handle character specifiers
        if (type.kind() == NewDataType::Kind::Character) {
            if (_helper->nextIf(Token::Type::PunctuationColon)) {
                type.setCharacterEncoding(this->parseCharacterEncodingSpecifier());
            }

            return type;
        }

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

        return type;
    }

    NewDataType Parser::parseTypePostfixes(const NewDataType& type, bool optionalWrapping) {
        NewDataType newType(type);

        if (_helper->nextIf(Token::Type::OperatorNot)) {
            newType.setAccess(NewDataType::Access::ReadWrite);
        }

        if (_helper->nextIf(Token::Type::OperatorQuestionMark)) {
            if (optionalWrapping) {
                NewDataType ptr(NewDataType::Kind::NullablePointer);

                ptr.addSubtype(newType);
                ptr.setLabel(newType.label());

                newType = ptr;
            } else {
                assert(newType.kind() == NewDataType::Kind::Pointer);
                newType.setKind(NewDataType::Kind::NullablePointer);
            }
        }

        return newType;
    }

    NewDataType Parser::parsePointerType() {
        assert(_helper->next().type() == Token::Type::OperatorStar);

        NewDataType type = NewDataType(NewDataType::Kind::Pointer);

        type = this->parseTypePostfixes(type, false);

        type.addSubtype(this->parseType());

        return type;
    }

    NewDataType Parser::parseArrayType() {
        assert(_helper->next().type() == Token::Type::PunctuationOpenBracket);

        NewDataType type = NewDataType(NewDataType::Kind::Array);

        type.setArrayCount(this->parseIntegerSpecifier());

        if (!_helper->nextIf(Token::Type::PunctuationCloseBracket)) {
            assert(0 && "Message: array type element count should be followed by a close bracket");
        }

        type.addSubtype(this->parseType());

        return type;
    }

    NewDataType Parser::parseFunctionType(bool signature, std::vector<std::string>* references) {
        NewDataType type;
        Token::Type closingPunctuation = _helper->peek().closingCounterpart();

        switch (_helper->next().type()) {
            case Token::Type::PunctuationOpenParen:
                type = NewDataType(NewDataType::Kind::Function);
                break;
            case Token::Type::PunctuationOpenBrace:
                type = NewDataType(NewDataType::Kind::Closure);

                // make sure to define the environment pointer
                type.addParameter(NewDataType::wrapInPointer(NewDataType::Kind::Void));
                break;
            default:
                assert(0 && "Message: failed to parse a function type");
        }

        // check for ending punctuation
        if (_helper->nextIf(closingPunctuation)) {
            type.addReturn(NewDataType::Kind::Void);
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

            NewDataType paramType;

            if (signature) {
                std::string label = parseTypeIdentifierPair(paramType);
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
            type.addReturn(NewDataType::Kind::Void);
            return type;
        }

        // finally, we have returns
        NewDataType returnTuple = NewDataType(NewDataType::Kind::Tuple);

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

            NewDataType returnType = this->parseType();

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
                type.addReturn(NewDataType::Kind::Void);
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

    NewDataType Parser::parseFunctionSignatureType() {
        return this->parseFunctionType(true);
    }

    uint32_t Parser::parseIntegerSpecifier() {
        if (_helper->peek().type() != Token::Type::LiteralInteger) {
            assert(0 && "Message: width specifier being invalid");
            return 0;
        }

        return strtol(_helper->next().str().c_str(), NULL, 10);
    }

    std::string Parser::parseTypeIdentifierPair(NewDataType& type) {
        // depth could be zero for untyped variables
        uint32_t depth = this->peekDepthIfAtType();
        if (depth > 0) {
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

        assert(0 && "Message: Expected a type-identifier or identifier");

        return "";
    }

    NewDataType::Access Parser::parseAnnotationAccess() {
        if (_helper->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: annotation argument should be r/read, w/write, rw/readwrite, or n/none");
            return NewDataType::Access::None;
        }

        std::string str = _helper->next().str();
        if (str == "r" || str == "read") {
            return NewDataType::Access::Read;
        } else if (str == "w" || str == "write") {
            return NewDataType::Access::Write;
        } else if (str == "rw" || str == "readwrite") {
            return NewDataType::Access::ReadWrite;
        } else if (str == "n" || str == "none") {
            return NewDataType::Access::None;
        }

        assert(0 && "Message: annotation argument should be r/read, w/write, rw/readwrite, or n/none");
        return NewDataType::Access::None;
    }

    NewDataType::CharacterEncoding Parser::parseCharacterEncodingSpecifier() {
        assert(_helper->peek().type() == Token::Type::Identifier);

        auto it = _characterEncodingMap.find(_helper->next().str());

        if (it == _characterEncodingMap.cend()) {
            assert(0 && "Message about invalid character encoding");
            return NewDataType::CharacterEncoding::UTF8;
        }

        return it->second;
    }

    bool Parser::isAtIdentifierAvailableForDefinition() {
        if (_helper->peek().type() != Token::Type::Identifier) {
            std::cout << _helper->peek().str() << std::endl;
            assert(0 && "Message: expecting identifier");
        }

        return _context->identifierAvailableForDefinition(_helper->peek().str());
    }

    void Parser::addFunctionForParsing(FunctionDefinitionNode* node) {
        _functions.push_back(node);
    }
}
