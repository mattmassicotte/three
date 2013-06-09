#pragma once

#include "../ASTNode.h"

#include <vector>

namespace Language {
    class EnumerationNode : public ASTNode {
    public:
        static EnumerationNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string structureName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _name;
        std::vector<std::string> _identifiers;
    };
}
