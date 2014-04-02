#include "BriefAnnotationNode.h"
#include "compiler/Parser/NewParser.h"

namespace Three {
    BriefAnnotationNode* BriefAnnotationNode::parse(NewParser& parser) {
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
