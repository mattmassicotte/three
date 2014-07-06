#include "DestructuredAssignmentOperatorNode.h"
#include "TupleNode.h"

namespace Three {
    DestructuredAssignmentOperatorNode* DestructuredAssignmentOperatorNode::parse(Parser& parser, ASTNode* firstVariable) {
        assert(firstVariable);

        // first, create our identifier tuple, which is the target of the assignment
        ASTNode* childNode = TupleNode::parse(parser, firstVariable, false);
        if (!childNode) {
            assert(0 && "Message: unable to parse tuple for destructured assignment");
            return nullptr;
        }

        if (!parser.helper()->nextIf(Token::Type::OperatorAssign)) {
            assert(0 && "Message: unable to parse assign operator from destructured assignment");
            return nullptr;
        }

        DestructuredAssignmentOperatorNode* node = new DestructuredAssignmentOperatorNode();

        node->addChild(childNode);

        // next, parse the source, which could be a tuple itself
        childNode = parser.parseExpressionWithTuples();
        if (!childNode) {
            assert(0 && "Message: unable to parse right side of destructured assignment");
            return nullptr;
        }

        node->addChild(childNode);

        return node;
    }

    std::string DestructuredAssignmentOperatorNode::nodeName() const {
        return "Destructured Assigment";
    }

    void DestructuredAssignmentOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
