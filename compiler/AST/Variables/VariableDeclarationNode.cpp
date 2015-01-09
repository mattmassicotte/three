#include "VariableDeclarationNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/Scope.h"

#include <assert.h>

namespace Three {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser, bool global) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        if (!VariableDeclarationNode::parseVariable(parser, *node, global)) {
            delete node;
            node = nullptr;
            return node;
        }

        if (global) {
            node->global = true;
            node->visibility = parser.context()->visibility();
            node->setStatement(true);
        }

        return node;
    }

    bool VariableDeclarationNode::parseVariable(Parser& parser, VariableDeclarationNode& node, bool global) {
        // Here's the form of a variable declaration:
        // (<type>) var (= <expression>)\n

        // TODO: this is fairly messy
        node._name = parser.parseTypeIdentifierPair(node._declaredType);
        if (node._name.size() == 0) {
            return false;
        }

        node._declaredType.setLabel(node._name);

        if (parser.helper()->nextIf(Token::Type::OperatorAssign)) {
            node._initializerExpression = parser.parseExpression();
        }

        // apply the namespace here
        QualifiedName qualifiedName = parser.context()->scope()->fullNamespace();
        qualifiedName.setName(node.name());

        node._variable = new Variable();

        node._variable->name = qualifiedName;
        node._variable->type = node.dataType();
        node._variable->global = global;

        // make this variable scoped *only* if it is not global
        return parser.context()->defineVariable(node._variable, global == false);
    }

    VariableDeclarationNode::VariableDeclarationNode() :
        _initializerExpression(nullptr),
        _variable(nullptr),
        global(false),
        visibility(TranslationUnit::Visibility::None) {
    }

    VariableDeclarationNode::~VariableDeclarationNode() {
        if (_initializerExpression) {
            delete _initializerExpression;
        }

        if (_variable) {
            delete _variable;
        }
    }

    DataType VariableDeclarationNode::dataType() const {
        return _declaredType;
    }

    std::string VariableDeclarationNode::nodeName() const {
        return "Variable Declaration";
    }

    void VariableDeclarationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string VariableDeclarationNode::name() const {
        return _name;
    }

    ASTNode* VariableDeclarationNode::initializerExpression() const {
        return _initializerExpression;
    }

    Variable* VariableDeclarationNode::variable() const {
        return _variable;
    }
}
