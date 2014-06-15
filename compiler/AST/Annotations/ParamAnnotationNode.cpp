#include "ParamAnnotationNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    ParamAnnotationNode* ParamAnnotationNode::parse(Parser& parser) {
        ParamAnnotationNode* node = new ParamAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationParam)) {
            assert(0 && "Message: Failed to parse an @summary annotation");
        }

        // there might be additional annotations here
        parser.helper()->parseUntil(false, [&] (const Token& token) {
            if (token.isAnnotation()) {
                node->addChild(AnnotationNode::parse(parser));
                return false;
            }

            return true;
        });

        node->parseText(parser);

        return node;
    }

    std::string ParamAnnotationNode::nodeName() const {
        return "Param Annotation";
    }

    void ParamAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
