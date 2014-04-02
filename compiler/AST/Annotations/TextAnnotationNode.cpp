#include "TextAnnotationNode.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/Lexer/Lexer.h"

#include <sstream>

namespace Three {
    std::string TextAnnotationNode::nodeName() const {
        return "Text Annotation";
    }

    bool TextAnnotationNode::parseText(NewParser& parser) {
        // This is actually quite complex, because we want to perserve
        // the contents exactly as entered. To do this, we need to be careful
        // with whitespace handling.

        std::stringstream s;
        bool hitNewline = false;
        bool filteringState = parser.helper()->lexer()->filterWhitespace();

        // We want to filter out leading whitespace, but it is required. This is an easy way.
        parser.helper()->lexer()->setFilterWhitespace(false);
        if (!parser.helper()->nextIf(Token::Type::Whitespace)) {
            assert(0 && "Leading whitespace for a text span is required");
            return false;
        }

        for (;;) {
            switch (parser.helper()->peek().type()) {
                case Token::Type::Newline:
                    hitNewline = true;
                    parser.helper()->next();
                    break;
                case Token::Type::Undefined:
                case Token::Type::EndOfInput:
                    // restore handling state
                    parser.helper()->lexer()->setFilterWhitespace(filteringState);

                    // we should have terminated with a newline
                    if (hitNewline) {
                        _text = s.str();
                        return true;
                    }

                    return false;
                case Token::Type::AnnotationBrief:
                case Token::Type::AnnotationSummary:
                case Token::Type::AnnotationParam:
                case Token::Type::AnnotationReturn:
                case Token::Type::KeywordDef:
                    // all tokens that effectively terminate this span of text
                    if (hitNewline) {
                        _text = s.str();
                        parser.helper()->lexer()->setFilterWhitespace(filteringState);
                        return true;
                    }
                    // intentional fallthrough
                default:
                    hitNewline = false;
                    s << parser.helper()->next().str();
                    break;
            }
        }

        parser.helper()->lexer()->setFilterWhitespace(filteringState);

        return false;
    }

    std::string TextAnnotationNode::text() const {
        return _text;
    }
}
