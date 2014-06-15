#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class VisibilityNode : public ASTNode {
    public:
        static VisibilityNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        virtual std::string name() const;

        TranslationUnit::Visibility type() const;

    private:
        TranslationUnit::Visibility _type;
    };
}
