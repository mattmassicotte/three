#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/NewVariable.h"
#include "compiler/constructs/TranslationUnit.h"

namespace Three {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser, bool createVariable = true);
        static VariableDeclarationNode* parseGlobal(Parser& parser);

    private:
        static void parseVariable(Parser& parser, VariableDeclarationNode& node, bool createVariable = true);

    public:
        VariableDeclarationNode();
        virtual ~VariableDeclarationNode();

        NewDataType dataType() const;
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        ASTNode* initializerExpression() const;

        NewVariable* variable() const;

        bool global;
        TranslationUnit::Visibility visibility;

    private:
        ASTNode* _initializerExpression;
        NewDataType _declaredType;
        std::string _name;
        NewVariable* _variable;
    };
}
