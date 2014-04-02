#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/Constructs/TranslationUnit.h"

namespace Three {
    class CompositeTypeDefinitionNode : public ASTNode {
    public:
        static CompositeTypeDefinitionNode* parse(NewParser& parser);

    public:
        std::string name() const;
        NewDataType definedType() const;

        TranslationUnit::Visibility visibility;

    protected:
        virtual void setParsedIntegerSpecifier(NewParser& parser, uint32_t value) = 0;

    private:
        std::string _name;
        NewDataType _definedType;
    };
}
