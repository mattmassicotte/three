#include "SwitchNode.h"
#include "CaseNode.h"
#include "ElseNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    SwitchNode* SwitchNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordSwitch));

        SwitchNode* node = new SwitchNode();

        node->_argumentNode = parser.parseExpression();
        node->_elseNode = nullptr;

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: switch argument must be followed by a newline");
            return node;
        }

        bool success = parser.parseBodyWithElse("switch", &node->_elseNode, [&] () {
            node->addChild(CaseNode::parse(parser));
        });

        if (!success) {
            assert(0 && "Message: unable to parse switch body");
        }

        return node;
    }

    std::string SwitchNode::nodeName() const {
        return "Switch";
    }

    std::string SwitchNode::name() const {
        return "Switch";
    }

    void SwitchNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* SwitchNode::argument() const {
        return _argumentNode;
    }

    ASTNode* SwitchNode::elseNode() const {
        return _elseNode;
    }
}
