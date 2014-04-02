#include "ReturnAnnotationNode.h"
#include "compiler/Parser/NewParser.h"

namespace Three {
    ReturnAnnotationNode* ReturnAnnotationNode::parse(NewParser& parser) {
        ReturnAnnotationNode* node = new ReturnAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationReturn)) {
            assert(0 && "Message: Failed to parse a @return annotation");
        }

        node->parseText(parser);

        return node;
    }

    std::string ReturnAnnotationNode::nodeName() const {
        return "Return Annotation";
    }

    void ReturnAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
