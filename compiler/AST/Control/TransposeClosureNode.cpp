#include "TransposeClosureNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/AST/Operators/Callable/CallableOperatorNode.h"

#include <assert.h>

namespace Three {
    TransposeClosureNode* TransposeClosureNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordTransposeClosure));

        TransposeClosureNode* node = new TransposeClosureNode();

        node->addChild(parser.parseExpressionElement());

        // create actual closure here
        // ClosureNode* closure = new ClosureNode();
        // 
        // closure->_name = parser.context()->scope()->scopedName("closure");

        return node;
    }

    std::string TransposeClosureNode::nodeName() const {
        return "Transpose Closure Node";
    }

    std::string TransposeClosureNode::name() const {
        return "Transpose Closure";
    }

    DataType TransposeClosureNode::dataType() const {
        const auto callable = dynamic_cast<CallableOperatorNode*>(this->childAtIndex(0));

        const auto paramCount = callable->receiverDataType().parameterCount();
        assert(paramCount > 0);

        const auto closure = callable->receiverDataType().parameterAtIndex(paramCount - 1);

        assert(closure.kind() == DataType::Kind::Closure);

        return closure.returnType();
    }

    void TransposeClosureNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
