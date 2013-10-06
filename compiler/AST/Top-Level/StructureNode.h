#pragma once

#include "DefinitionNode.h"
#include "../../Constructs/DataType.h"

namespace Three {
    class StructureNode : public DefinitionNode {
    public:
        static StructureNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        uint32_t packing() const;
        std::string structureName() const;

        void codeGen(CSourceContext& context);

    private:
        uint32_t _packing;
        DataType* _type;
    };
}
