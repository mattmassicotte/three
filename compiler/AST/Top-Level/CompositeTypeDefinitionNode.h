#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/Constructs/TranslationUnit.h"
#include "compiler/Constructs/QualifiedName.h"

namespace Three {
    class CompositeTypeDefinitionNode : public ASTNode {
    public:
        static CompositeTypeDefinitionNode* parse(Parser& parser);

    private:
        static bool defineConstantForEnumerationMember(const CompositeTypeDefinitionNode& node, const class CompositeTypeMemberNode& member, Parser& parser);

    public:
        std::string name() const;
        DataType definedType() const;

        TranslationUnit::Visibility visibility;

        std::string fullName() const;
        QualifiedName qualifiedName() const;

    protected:
        virtual void setParsedIntegerSpecifier(Parser& parser, uint32_t value) = 0;

    private:
        QualifiedName _name;
        DataType _definedType;
    };
}
