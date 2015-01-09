#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/Variable.h"
#include "compiler/constructs/TranslationUnit.h"

namespace Three {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser, bool global = false);

    private:
        static bool parseVariable(Parser& parser, VariableDeclarationNode& node, bool global);

    public:
        VariableDeclarationNode();
        virtual ~VariableDeclarationNode();

        DataType dataType() const;
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        ASTNode* initializerExpression() const;

        Variable* variable() const;

        bool global;
        TranslationUnit::Visibility visibility;

    private:
        ASTNode* _initializerExpression;
        DataType _declaredType;
        std::string _name;
        Variable* _variable;
    };
}
