#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/NewVariable.h"

namespace Three {
    class VariableNode : public ASTNode {
    public:
        static VariableNode* parse(NewParser& parser, const std::string& identifier);

    public:
        VariableNode();

        NewDataType dataType() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        std::string str() const;

        NewVariable* newVariable() const;
        bool closed() const;
        bool referenced() const;

    private:
        NewVariable* _newVariable;
        bool      _referenced;
        bool      _closed;
        std::string _name;
    };
}
