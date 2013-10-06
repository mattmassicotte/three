#pragma once

#include "DefinitionNode.h"
#include "../../Constructs/TypeReference.h"

namespace Three {
    class TypeDefinitionNode : public DefinitionNode {
    public:
        static TypeDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        TypeReference sourceType() const;
        std::string destTypeName() const;

        void codeGen(CSourceContext& context);

    private:
        TypeReference _sourceType;
        std::string   _destTypeName;
    };
}
