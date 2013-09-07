#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class NamespaceNode : public ASTNode {
    public:
        static NamespaceNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string namespaceName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _name;
    };
}
