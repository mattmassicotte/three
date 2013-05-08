#pragma once

#include "ASTNode.h"

namespace Language {
    class FunctionDefinitionNode : public ASTNode {
    public:
        static std::string parseType(Parser& parser) ;
        static std::string parseParameter(Parser& parser);

        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setFunctionName(const std::string& name);
        std::string functionName() const;

    private:
        std::string _functionName;

        std::vector<std::string> _parameterTypes;
        std::string              _returnType;
    };
}
