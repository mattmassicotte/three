#pragma once

#include "ASTNode.h"

namespace Language {
    class ReturnNode : public ASTNode {
    public:
        static ReturnNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        virtual void renderCCode(std::stringstream& stream, uint32_t indentation);
    };
}
