#include "IndexerNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    IndexerNode* IndexerNode::parse(Parser& parser, ASTNode* operand) {
        IndexerNode* node = new Three::IndexerNode();

        assert(parser.next().str() == "[");

        node->addChild(operand);
        node->addChild(parser.parseExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseBracket);

        return node;
    }
    
    std::string IndexerNode::name() const {
        return "Indexer";
    }

    void IndexerNode::codeGen(CSourceContext& context) {
        this->childAtIndex(0)->codeGen(context);

        context << "[";

        // index expression
        this->childAtIndex(1)->codeGen(context);

        context << "]";
    }
}
