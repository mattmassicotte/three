#include "TupleNode.h"

namespace Three {
    TupleNode* TupleNode::parse(Parser& parser, ASTNode* firstElement, bool fullExpressions, const NewDataType* expectedType) {
        assert(firstElement);

        TupleNode* node = new TupleNode();

        node->addChild(firstElement);

        // we should be at a comma right now
        if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
            assert(0 && "Message: A Tuple should have more than one element (comma missing)");
            return nullptr;
        }

        parser.helper()->parseUntil(false, [&] (const Token& token) {
            ASTNode* nextElement = nullptr;

            switch (token.type()) {
                case Token::Type::Newline:
                case Token::Type::KeywordIf: // for tailing ifs
                case Token::Type::PunctuationCloseParen: // for end of function arguments
                    return true;
                default:
                    // This bit is to allow for full tuples, with expressions, or just
                    // for expressionless identifiers only
                    if (fullExpressions) {
                        nextElement = parser.parseExpression();
                    } else {
                        nextElement = parser.parseExpressionIdentifier();
                    }

                    if (!nextElement) {
                        return true; // we're done here
                    }

                    node->addChild(nextElement);
                    break;
            }

            // if the next token is a comma, we have more elements to parse,
            // and if not, we're done
            return !parser.helper()->nextIf(Token::Type::PunctuationComma);
        });

        if (node->childCount() == 0) {
            assert(0 && "Message: failed to parse tuple - zero children");
            return nullptr;
        }

        if (expectedType) {
            node->_type = *expectedType;
        } else {
            // we now need to construct the type
            NewDataType type = NewDataType(NewDataType::Kind::Tuple);
            node->eachChildWithLast([&] (const ASTNode* child, bool last) {
                type.addSubtype(child->dataType());
            });

            node->_type = type;
        }

        // we must have more than one child
        assert(node->childCount() > 0);

        assert(node->childCount() == node->_type.subtypeCount() && "Message: tuple not compatible with expected type");

        return node;
    }

    NewDataType TupleNode::dataType() const {
        return _type;
    }

    std::string TupleNode::nodeName() const {
        return "Tuple";
    }

    void TupleNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string TupleNode::name() const {
        return "Tuple!!!";
    }

    std::string TupleNode::str() const {
        return this->nodeName();
    }
}
