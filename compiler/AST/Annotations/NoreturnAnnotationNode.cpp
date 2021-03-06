#include "NoreturnAnnotationNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    NoreturnAnnotationNode* NoreturnAnnotationNode::parse(Parser& parser) {
        NoreturnAnnotationNode* node = new NoreturnAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationNoreturn)) {
            assert(0 && "Message: Failed to parse an @summary annotation");
        }

        return node;
    }

    std::string NoreturnAnnotationNode::nodeName() const {
        return "No Return Annotation";
    }

    void NoreturnAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
