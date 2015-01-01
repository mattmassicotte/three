#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/Constructs/QualifiedName.h"

namespace Three {
    class CompositeTypeMemberNode : public ASTNode {
    public:
        static CompositeTypeMemberNode* parse(Parser& parser);

    public:
        DataType dataType() const;
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        QualifiedName qualifiedName() const;

    private:
        DataType _declaredType;
        QualifiedName _name;
    };
}
