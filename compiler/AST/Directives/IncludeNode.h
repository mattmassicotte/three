#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class IncludeNode : public ASTNode {
    public:
        static IncludeNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string headerName() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _headerName;
        TranslationUnit::Visibility _visibility;
    };
}
