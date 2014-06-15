#include "ReadAnnotationNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    ReadAnnotationNode* ReadAnnotationNode::parse(Parser& parser) {
        ReadAnnotationNode* node = new ReadAnnotationNode();

        if (!parser.helper()->nextIf(Token::Type::AnnotationRead)) {
            assert(0 && "Message: Failed to parse an @read annotation");
        }

        parser.parseParentheses([&] () {
            // TODO: totally bogus
            parser.helper()->parseUntil(false, [&] (const Token& token) {
                if (token.type() == Token::Type::PunctuationCloseParen) {
                    return true;
                }

                parser.helper()->next();

                return false;
            });
        });

        return node;
    }

    std::string ReadAnnotationNode::nodeName() const {
        return "Read Annotation";
    }

    void ReadAnnotationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
