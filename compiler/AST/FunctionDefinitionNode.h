#pragma once

#include "ASTNode.h"

namespace Language {
    class FunctionDefinitionNode : public ASTNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setFunctionName(const std::string& name);
        std::string functionName() const;

    private:
        std::string _functionName;
    };
}
