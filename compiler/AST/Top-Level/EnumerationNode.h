#pragma once

#include "DefinitionNode.h"
#include "../../Constructs/DataType.h"

#include <vector>

namespace Three {
    class EnumerationNode : public DefinitionNode {
    public:
        static EnumerationNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        std::string structureName() const;
        std::vector<std::string> identifiers() const;
        DataType* type() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _name;
        std::vector<std::string> _identifiers;
        DataType* _type;
    };
}
