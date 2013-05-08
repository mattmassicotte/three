#pragma once

#include "ASTNode.h"

namespace Language {
    class FunctionCallNode : public ASTNode {
    public:
        static FunctionCallNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setFunctionName(const std::string& name);
        std::string functionName() const;

    private:
        std::string _functionName;
    };
}
