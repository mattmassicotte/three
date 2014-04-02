#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class ExternalSourceNode : public ASTNode {
    public:
        void accept(ASTVisitor& visitor);

        std::string argument() const;

        virtual std::string headerName() const = 0;
        virtual bool relative() const = 0;
        TranslationUnit::Visibility visibility() const;

    protected:
        std::string _argument;

        TranslationUnit::Visibility _visibility;
    };
}
