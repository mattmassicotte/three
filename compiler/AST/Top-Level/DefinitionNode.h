#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

using namespace Language;

namespace Three {
    class DefinitionNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser, bool inModule);

    public:
        DefinitionNode();

        TranslationUnit::Visibility visibility() const;

    private:
        TranslationUnit::Visibility _visibility;
    };
}
