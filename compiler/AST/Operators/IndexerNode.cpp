#include "IndexerNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    IndexerNode* IndexerNode::parse(Parser& parser, ASTNode* operand) {
        if (!parser.helper()->nextIf(Token::Type::PunctuationOpenBracket)) {
            assert(0 && "Message: Indexer should start with a [");
            return nullptr;
        }

        IndexerNode* node = new Three::IndexerNode();

        node->addChild(operand);
        node->addChild(parser.parseExpression());

        if (!parser.helper()->nextIf(Token::Type::PunctuationCloseBracket)) {
            assert(0 && "Message: Indexer should end with a ]");
        }

        return node;
    }

    std::string IndexerNode::nodeName() const {
        return "Indexer Operator";
    }

    void IndexerNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
