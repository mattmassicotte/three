#pragma once

#include "../ASTNode.h"

namespace Language {
    class StructureNode : public ASTNode {
    public:
        static StructureNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        uint32_t packing() const;
        std::string structureName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        void renderCSourcePackingStart(CSourceContext& context);
        void renderCSourcePackingStop(CSourceContext& context);

    private:
        uint32_t _packing;
        std::string _name;
    };
}
