#include "AnnotationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    AnnotationNode* AnnotationNode::parse(Parser& parser) {
        std::string name;

        AnnotationNode* node = new AnnotationNode();

        node->_annotation = NULL;

        name = parser.next().str();
        if (name == "@noreturn") {
            node->_annotation = new NoReturnAnnotation();
        } else if (name == "@inline") {
            
            assert(parser.next().type() == Token::Type::PunctuationOpenParen);

            std::string value = parser.next().str();
            if (value == "always") {
                node->_annotation = new InlineAnnotation(InlineAnnotation::Type::Always);
            } else if (value == "never") {
                node->_annotation = new InlineAnnotation(InlineAnnotation::Type::Never);
            } else if (value == "allowed"){
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
        } else if (name == "@c") {
            assert(parser.next().type() == Token::Type::PunctuationOpenParen);

            std::string value = parser.next().str();
            if (value == "notypedef") {
                node->_annotation = new CSourceAnnotation(CSourceAnnotation::Type::NoTypedef);
            } else {
                assert(0 && "Unrecognized @c annotation type");
            }

            assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        } else {
            assert(0 && "Unrecognized annotation");
        }

        node->_annotation->annotationName = name;

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

    void AnnotationNode::codeGen(CSourceContext& context) {
        context.current()->addHeader(false, "three/runtime/annotations.h");
    }
}
