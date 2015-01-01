#include "CompositeTypeMemberNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    CompositeTypeMemberNode* CompositeTypeMemberNode::parse(Parser& parser) {
        CompositeTypeMemberNode* node = new CompositeTypeMemberNode();

        node->_name = QualifiedName(parser.parseTypeIdentifierPair(node->_declaredType));
        if (node->name().size() == 0) {
            delete node;
            return nullptr;
        }

        node->_declaredType.setLabel(node->name());

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: A composite-type's member must be followed by a newline");
        }

        return node;
    }

    DataType CompositeTypeMemberNode::dataType() const {
        return _declaredType;
    }

    std::string CompositeTypeMemberNode::nodeName() const {
        return "Composite Type Member";
    }

    void CompositeTypeMemberNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string CompositeTypeMemberNode::name() const {
        return _name.lastComponent();
    }

    QualifiedName CompositeTypeMemberNode::qualifiedName() const {
        return _name;
    }
}
