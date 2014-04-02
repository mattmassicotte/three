#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class DefinitionNode : public ASTNode {
    public:
        DefinitionNode();

        void setVisibility(TranslationUnit::Visibility visibility);
        TranslationUnit::Visibility visibility() const;

    private:
        TranslationUnit::Visibility _visibility;
    };
}
