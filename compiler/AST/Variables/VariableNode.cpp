#include "VariableNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/NewScope.h"
#include "LocalVariableNode.h"
#include "FunctionVariableNode.h"
#include "ReferencedVariableNode.h"
#include "CapturedVariableNode.h"
#include "SelfVariableNode.h"

#include "compiler/Messages/UnrecognizedVariableMessage.h"

#include <assert.h>

namespace Three {
    VariableNode* VariableNode::parse(Parser& parser, const QualifiedName& name) {
        VariableNode* node = nullptr;

        NewVariable* variable = parser.context()->variableForName(name.to_s());
        if (!variable) {
            parser.context()->addMessage(new UnrecognizedVariableMessage(name.to_s()));
            return nullptr;
        }

        if (parser.context()->functionForName(name.to_s()).kind() != DataType::Kind::Undefined) {
            node = new FunctionVariableNode();
        } else if (parser.context()->scope()->referencedVariable(name.to_s())) {
            node = new ReferencedVariableNode();
        } else if (parser.context()->scope()->capturedVariable(name.to_s())) {
            node = new CapturedVariableNode();

            // TODO: this is weird
            parser.context()->scope()->captureVariable(name.to_s());
        } else if (name.to_s() == "self") {
            node = new SelfVariableNode();
        } else {
            node = new LocalVariableNode();
        }

        node->_name = name.to_s();
        node->_newVariable= variable;

        return node;
    }

    VariableNode::VariableNode() : _referenced(false), _closed(false), _newVariable(nullptr) {
    }

    DataType VariableNode::dataType() const {
        if (!_newVariable) {
            std::cout << "variable undefined!" << std::endl;
            return DataType();
        }

        return _newVariable->type;
    }

    std::string VariableNode::nodeName() const {
        return "Variable";
    }

    void VariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string VariableNode::name() const {
        return _name;
    }

    std::string VariableNode::str() const {
        return this->nodeName() + ": " + this->name();
    }

    NewVariable* VariableNode::newVariable() const {
        return _newVariable;
    }

    bool VariableNode::closed() const {
        return _closed;
    }

    bool VariableNode::referenced() const {
        return _referenced;
    }
}
