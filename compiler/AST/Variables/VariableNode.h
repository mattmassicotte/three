#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/Variable.h"
#include "compiler/constructs/QualifiedName.h"

namespace Three {
    class VariableNode : public ASTNode {
    public:
        static VariableNode* parse(Parser& parser, const QualifiedName& name);

    public:
        VariableNode();

        DataType dataType() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        std::string str() const;

        Variable* newVariable() const;
        bool closed() const;
        bool referenced() const;

    private:
        Variable* _newVariable;
        bool      _referenced;
        bool      _closed;
        std::string _name;
    };
}
