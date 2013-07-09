#include "AnnotationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    AnnotationNode* AnnotationNode::parse(Parser& parser) {
        std::string name;

        AnnotationNode* node = new AnnotationNode();

        node->_annotation = NULL;

        name = parser.next().str();
        if (name == "@noreturn") {
            node->_annotation = new NoReturnAnnotation();
        } else if (name == "@inline") {
            assert(parser.next().type() == Token::Type::PunctuationOpenParen);

            name = parser.next().str();
            if (name == "always") {
                node->_annotation = new InlineAnnotation(InlineAnnotation::Type::Always);
            } else if (name == "never") {
                node->_annotation = new InlineAnnotation(InlineAnnotation::Type::Never);
            } else if (name == "allowed"){
                node->_annotation = new InlineAnnotation(InlineAnnotation::Type::Allowed);
            } else {
                assert(0 && "Unrecognized @inline annotation type");
            }

            assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        } else if (name == "@common") {
            node->_annotation = new CommonAnnotation();

            assert(parser.next().type() == Token::Type::PunctuationOpenParen);

            node->addChild(parser.parseExpression());

            assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        } else {
            assert(0 && "Unrecognized annotation");
        }

        parser.currentScope()->addAnnotation(node->annotation());

        return node;
    }

    AnnotationNode::~AnnotationNode() {
        if (_annotation) {
            delete _annotation;
        }
    }

    std::string AnnotationNode::name() const {
        return "Annotation";
    }

    Annotation* AnnotationNode::annotation() const {
        return _annotation;
    }

    void AnnotationNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("three/runtime/annotations.h");
    }
}
