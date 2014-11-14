#pragma once

#include "ParseContext.h"
#include "ParseHelper.h"
#include "compiler/constructs/DataType.h"

#include <string>
#include <map>

namespace Three {
    class ASTNode;
    class Lexer;
    class FunctionDefinitionNode;
    class QualifiedName;
}

namespace Three {
    class Parser {
    public:
        static bool parse(const char* inputPath, ParseContext* context);

    public:
        Parser();
        virtual ~Parser();

    private:
        Parser(const Parser& other);
        Parser(Parser&& other);
        Parser& operator=(Parser other);
        Parser& operator=(Parser&& other);

    public:
        ParseContext* context() const;
        ParseHelper* helper() const;
        bool verbose() const;

    public:
        bool parse(Lexer* lexer, ParseContext* context);
        bool parseFunctionBodies(ParseContext* context);

        ASTNode* parseTopLevelNode(bool* success);
        ASTNode* parseStatement();
        ASTNode* parseExpressionStatement();
        ASTNode* parseExpressionWithTuples(const DataType* expectedType = nullptr);
        ASTNode* parseExpression(uint32_t precedence=Token::MinimumPrecedence);
        ASTNode* parseExpressionElement();
        ASTNode* parseExpressionElementWithoutTailingOperators();
        ASTNode* parseExpressionIdentifier();
        bool parseParentheses(std::function<void (void)> func);
        std::string parseMultiPartIdentifier();
        QualifiedName parseMultiPartIdentifierComponents();
        bool parseBodyWithElse(const std::string& label, ASTNode** elseNode, std::function<void (void)> func);
        bool parseBodyWithElse(ASTNode* node, ASTNode** elseNode);

        uint32_t peekDepthIfAtType(uint32_t peekDepth = 1);
        bool isAtType();
        bool peekType(unsigned int* peekDepth);
        bool peekTypePrefixes(unsigned int* peekDepth);
        bool peekTypePostfixes(unsigned int* peekDepth);
        bool peekTypeAnnotations(unsigned int* peekDepth);
        bool peekFunctionType(unsigned int* peekDepth);
        bool peekNonFunctionType(unsigned int* peekDepth);
        bool peekScopedIdentifier(unsigned int* peekDepth, QualifiedName& peekedName);
        DataType parseType(bool genericParam = false);
        DataType parseAndApplyTypeAnnotations(bool genericParam = false);
        DataType parseTypeWithoutAnnotations(bool genericParam = false);
        void parseTypeSpecifiers(DataType& type);
        DataType parseTypePostfixes(const DataType& type);
        DataType parsePointerType(bool genericParam = false);
        DataType parseArrayType(bool genericParam = false);
        DataType parseFunctionType(bool signature = false, std::vector<std::string>* references = nullptr);
        DataType parseFunctionSignatureType();
        uint32_t parseIntegerSpecifier();
        std::string parseTypeIdentifierPair(DataType& type);
        DataType::Access parseAnnotationAccess();
        DataType::CharacterEncoding parseCharacterEncodingSpecifier();
        bool parseGenericParameters(DataType& type);

        bool isAtIdentifierAvailableForDefinition();

        void addFunctionForParsing(FunctionDefinitionNode* node);

    private:
        void startParse();
        void parseFunctionBody(FunctionDefinitionNode* node);

    private:
        ParseContext* _context;
        ParseHelper* _helper;

        std::map<std::string, DataType::CharacterEncoding> _characterEncodingMap;

        std::vector<FunctionDefinitionNode*> _functions;
    };
}
