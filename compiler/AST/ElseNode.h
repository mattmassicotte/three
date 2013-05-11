#pragma once

#include "ASTNode.h"

namespace Language {
    class ElseNode : public ASTNode {
    public:
        static ElseNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        virtual void renderCCode(std::stringstream& stream, uint32_t indentation);
    };
}
