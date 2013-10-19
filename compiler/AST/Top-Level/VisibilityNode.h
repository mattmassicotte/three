#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class VisibilityNode : public ASTNode {
    public:
        static VisibilityNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        TranslationUnit::Visibility type() const;

        void codeGen(CSourceContext& context);

    private:
        TranslationUnit::Visibility _type;
    };
}
