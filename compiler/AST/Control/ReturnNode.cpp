#include "ReturnNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>

namespace Three {
    ReturnNode* ReturnNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordReturn));

        ReturnNode* node = new ReturnNode();

        node->_transactionName = parser.context()->scope()->currentScopedName("tx");

        NewDataType returnType = parser.context()->scope()->currentFunctionReturnType();

        ASTNode* child = nullptr;

        switch (parser.helper()->peek().type()) {
            case Token::Type::Newline:
            case Token::Type::KeywordIf: // for tailing ifs
                break;
            default:
                child = parser.parseExpressionWithTuples(&returnType);
                if (child) {
                    node->addChild(child);
                }
                break;
        }

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
