#pragma once

#include "../ASTNode.h"

#include <vector>

namespace Three {
    class EnumerationNode : public ASTNode {
    public:
        static EnumerationNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string structureName() const;

    private:
        std::string _name;
        std::vector<std::string> _identifiers;
    };
}
