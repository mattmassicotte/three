#include "EnsureNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    EnsureNode* EnsureNode::parse(Parser& parser) {
        EnsureNode* node = new EnsureNode();

        assert(parser.next().type() == Token::Type::KeywordEnsure);
        parser.parseNewline();

        // parse the body
        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    std::string EnsureNode::name() const {
        return "Ensure";
    }

    void EnsureNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
