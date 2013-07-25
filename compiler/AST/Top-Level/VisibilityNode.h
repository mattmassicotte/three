#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

using namespace Language;

namespace Three {
    class VisibilityNode : public ASTNode {
    public:
        static VisibilityNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        TranslationUnit::Visibility type() const;

        void codeGenCSource(CSourceContext& context);

    private:
        TranslationUnit::Visibility _type;
    };
}
