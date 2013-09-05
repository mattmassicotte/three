#pragma once

#include "DefinitionNode.h"

namespace Three {
    class ValueDefinitionNode : public DefinitionNode {
    public:
        static ValueDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string definedName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _definedName;
    };
}
