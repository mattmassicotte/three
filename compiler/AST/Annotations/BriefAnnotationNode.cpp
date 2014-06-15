#include "BriefAnnotationNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    BriefAnnotationNode* BriefAnnotationNode::parse(Parser& parser) {
        BriefAnnotationNode* node = new BriefAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationBrief)) {
            assert(0 && "Message: Failed to parse an @brief annotation");
        }

        node->parseText(parser);

        return node;
    }

    std::string BriefAnnotationNode::nodeName() const {
        return "Brief Annotation";
    }

    void BriefAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
