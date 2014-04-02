#pragma once

#include "DefinitionNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/Lexer/Token.h"

namespace Three {
    class NewScope;
}

namespace Three {
    class FunctionDefinitionNode : public DefinitionNode {
    public:
        static FunctionDefinitionNode* parse(NewParser& parser);
        static bool bufferFunctionBody(NewParser& parser, std::stringstream& stream);
        static bool bufferOpenToCloseToken(NewParser& parser, std::stringstream& stream, Token::Type closingType, bool parseClosing);
        static bool scanToFirstToken(NewParser& parser, std::stringstream& stream, bool firstCall);

    public:
        std::string nodeName() const;
        std::string name() const;
        std::string str() const;
        void accept(ASTVisitor& visitor);

        NewDataType functionType() const;
        NewDataType methodOnType() const;
        bool isMethod() const;

        std::stringstream* bodyStream();
        bool parseBody(NewParser& parser);

        std::string fullName() const;
        std::vector<std::string> namespaceComponents;

    private:
        void defineParameterVariablesInScope(NewScope* scope);

    private:
        NewDataType _functionType;
        NewDataType _methodOnType;
        std::string _name;
        std::stringstream _bodyString;
    };
}
