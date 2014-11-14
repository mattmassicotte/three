#include "VariableDeclarationNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>

namespace Three {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser, bool createVariable) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        if (!VariableDeclarationNode::parseVariable(parser, *node, createVariable)) {
            delete node;
            node = nullptr;
        }

        return node;
    }

    VariableDeclarationNode* VariableDeclarationNode::parseGlobal(Parser& parser) {
        VariableDeclarationNode* node = VariableDeclarationNode::parse(parser, true);
        if (!node) {
            return nullptr;
        }

        node->global = true;
        node->visibility = parser.context()->visibility();
        node->setStatement(true);

        return node;
    }

    bool VariableDeclarationNode::parseVariable(Parser& parser, VariableDeclarationNode& node, bool createVariable) {
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

        if (!createVariable) {
            return true;
        }

        node._variable = new NewVariable();

        node._variable->name = node.name();
        node._variable->type = node.dataType();

        return parser.context()->scope()->defineVariable(node._variable);
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

    NewVariable* VariableDeclarationNode::variable() const {
        return _variable;
    }
}
