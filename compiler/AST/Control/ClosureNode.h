#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/NewVariable.h"

namespace Three {
    class ClosureNode : public ASTNode {
    public:
        static ClosureNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        NewDataType dataType() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        void eachCapturedVariable(std::function<void (NewVariable*, bool, bool)> func) const;
        std::string closureName() const;
        bool hasReferences() const;
        NewDataType environmentStructureType() const;

    public:
        NewDataType environmentType;

    private:
        NewDataType _dataType;
        std::string _name;
        std::string _environmentName;
        std::vector<NewVariable*> _capturedNewVariables;
        std::vector<NewVariable*> _referencedNewVariables;

        uint32_t _savedCSourceLocation;
    };
}
