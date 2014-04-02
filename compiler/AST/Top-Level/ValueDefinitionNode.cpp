#include "ValueDefinitionNode.h"

#include <assert.h>

namespace Three {
    // ValueDefinitionNode* ValueDefinitionNode::parse(OldParser& parser) {
    //     ValueDefinitionNode* node = new ValueDefinitionNode();
    // 
    //     assert(parser.peek().type() == Token::Identifier);
    // 
    //     node->_definedName = parser.next().str();
    // 
    //     parser.parseNewline();
    // 
    //     parser.currentModule()->addConstant(node->_definedName, node->_definedName);
    // 
    //     return node;
    // }

    std::string ValueDefinitionNode::name() const {
        return "ValueDefinition";
    }

    std::string ValueDefinitionNode::definedName() const  {
        return _definedName;
    }
}
