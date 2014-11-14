#pragma once

#include "DefinitionNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/QualifiedName.h"
#include "compiler/Lexer/Token.h"

namespace Three {
    class Scope;
}

namespace Three {
    class FunctionDefinitionNode : public DefinitionNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);
        static bool bufferFunctionBody(Parser& parser, std::stringstream& stream);
        static bool bufferOpenToCloseToken(Parser& parser, std::stringstream& stream);
        static bool scanToFirstToken(Parser& parser, std::stringstream& stream, bool firstCall);

    public:
        std::string nodeName() const;
        std::string name() const;
        std::string str() const;
        void accept(ASTVisitor& visitor);

        DataType functionType() const;
        DataType methodOnType() const;
        bool isMethod() const;

        std::stringstream* bodyStream();
        bool parseBody(Parser& parser);

        std::string fullName() const;

        void eachNamedReturn(std::function<void (const DataType&)> block) const;

    private:
        void defineVariableForReturnType(Scope* scope, const DataType& type) const;
        void defineParameterVariablesInScope(Scope* scope) const;

    private:
        DataType _functionType;
        DataType _methodOnType;
        QualifiedName _name;
        QualifiedName _scopedNamespace;
        std::stringstream _bodyString;
    };
}
