#include "ReturnNode.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>

namespace Three {
    ReturnNode* ReturnNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordReturn));

        ReturnNode* node = new ReturnNode();

        node->_transactionName = parser.context()->scope()->currentScopedName("tx");

        parser.helper()->parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::Newline:
                case Token::Type::KeywordIf: // for tailing ifs
                    return true;
                default:
                    node->addChild(parser.parseExpression());
                    break;
            }

            // if the next token is a comma, we have more return values to parse,
            // and if not, we're done
            return !parser.helper()->nextIf(Token::Type::PunctuationComma);
        });

        node->setStatement(true);

        return node;
    }

    std::string ReturnNode::nodeName() const {
        return "Return";
    }

    std::string ReturnNode::name() const {
        return "Return";
    }

    void ReturnNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    bool ReturnNode::endsTransaction() const {
        return this->transactionName().size() > 0;
    }

    std::string ReturnNode::transactionName() const {
        return _transactionName;
    }
}
