#pragma once

#include "CompositeTypeDefinitionNode.h"

#include <vector>

namespace Three {
    class EnumerationNode : public CompositeTypeDefinitionNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::vector<std::string> identifiers() const;

        uint32_t size() const;

        void eachMemberWithLast(std::function<void (const std::string&, bool)> func);

    protected:
        void setParsedIntegerSpecifier(NewParser& parser, uint32_t value);

    private:
        uint32_t _size;
        std::vector<std::string> _identifiers;
    };
}
