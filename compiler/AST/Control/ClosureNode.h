#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/Variable.h"

namespace Three {
    class ClosureNode : public ASTNode {
    public:
        static ClosureNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        DataType dataType() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        void eachCapturedVariable(std::function<void (Variable*, bool, bool)> func) const;
        std::string closureName() const;
        bool hasReferences() const;
        DataType environmentStructureType() const;

    public:
        DataType environmentType;

    private:
        DataType _dataType;
        std::string _name;
        std::string _environmentName;
        std::vector<Variable*> _capturedVariables;
        std::vector<Variable*> _referencedVariables;

        uint32_t _savedCSourceLocation;
    };
}
