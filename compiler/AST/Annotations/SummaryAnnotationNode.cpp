#include "SummaryAnnotationNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    SummaryAnnotationNode* SummaryAnnotationNode::parse(Parser& parser) {
        SummaryAnnotationNode* node = new SummaryAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationSummary)) {
            assert(0 && "Message: Failed to parse an @summary annotation");
        }

        node->parseText(parser);

        return node;
    }

    std::string SummaryAnnotationNode::nodeName() const {
        return "Summary Annotation";
    }

    void SummaryAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
