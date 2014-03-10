#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class ExternalSourceNode : public ASTNode {
    public:
        void accept(ASTVisitor& visitor);

        virtual std::string headerName() const = 0;
        virtual bool relative() const = 0;
        TranslationUnit::Visibility visibility() const;

    protected:
        TranslationUnit::Visibility _visibility;
    };
}
