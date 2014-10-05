#pragma once

#include "DefinitionNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/QualifiedName.h"
#include "compiler/Lexer/Token.h"

namespace Three {
    class NewScope;
}

namespace Three {
    class FunctionDefinitionNode : public DefinitionNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);
        static bool bufferFunctionBody(Parser& parser, std::stringstream& stream);
        static bool bufferOpenToCloseToken(Parser& parser, std::stringstream& stream, Token::Type closingType, bool parseClosing);
        static bool newBufferOpenToCloseToken(Parser& parser, std::stringstream& stream);
        static bool scanToFirstToken(Parser& parser, std::stringstream& stream, bool firstCall);

    public:
        std::string nodeName() const;
        std::string name() const;
        std::string str() const;
        void accept(ASTVisitor& visitor);

        NewDataType functionType() const;
        NewDataType methodOnType() const;
        bool isMethod() const;

        std::stringstream* bodyStream();
        bool parseBody(Parser& parser);

        std::string fullName() const;

        void eachNamedReturn(std::function<void (const NewDataType&)> block) const;

    private:
        void defineVariableForReturnType(NewScope* scope, const NewDataType& type) const;
        void defineParameterVariablesInScope(NewScope* scope) const;

    private:
        NewDataType _functionType;
        NewDataType _methodOnType;
        QualifiedName _name;
        QualifiedName _scopedNamespace;
        std::stringstream _bodyString;
    };
}
