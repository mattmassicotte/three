#pragma once

#include "ASTNode.h"

namespace Language {
    class FunctionDefinitionNode : public ASTNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
    };
}
