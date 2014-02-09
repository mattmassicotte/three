#include "VariableDeclarationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser, bool createVariable) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        node->global = false;
        node->_variable = new Variable();

        node->_variable->setType(parser.parseType());

        assert(parser.peek().type() == Token::Type::Identifier);
        node->_variable->setName(parser.next().str());

        if (createVariable) {
            parser.currentScope()->addVariable(node->_variable->name(), node->_variable);
        }

        if (parser.nextIf("=")) {
            node->_initializerExpression = parser.parseExpression();
        } else {
            node->_initializerExpression = nullptr;
        }

        // TODO: this isn't quite right.  These aren't always statements.  So far,
        // one place where they are not are in for loop start expressions
        node->setStatement(true);

        return node;
    }

    VariableDeclarationNode::~VariableDeclarationNode() {
        if (_initializerExpression) {
            delete _initializerExpression;
        }
    }

    std::string VariableDeclarationNode::name() const {
        return "VariableDeclaration";
    }

    TypeReference VariableDeclarationNode::nodeType() const {
        return _variable->type();
    }

    void VariableDeclarationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    Variable* VariableDeclarationNode::variable() const {
        return this->_variable;
    }

    ASTNode* VariableDeclarationNode::initializerExpression() const {
        return _initializerExpression;
    }

    void VariableDeclarationNode::codeGen(CSourceContext& context) {
        context << this->_variable->type().codeGen(this->_variable->name());

        if (this->initializerExpression()) {
            context << " = ";
            this->initializerExpression()->codeGen(context);
        }
    }
}
