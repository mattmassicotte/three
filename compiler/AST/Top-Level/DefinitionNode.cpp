#include "DefinitionNode.h"
#include "TypeDefinitionNode.h"
#include "FunctionDeclarationNode.h"
#include "FunctionDefinitionNode.h"
#include "ValueDefinitionNode.h"

#include <assert.h>

namespace Three {
    DefinitionNode::DefinitionNode() : _visibility(TranslationUnit::Visibility::External) {
    }

    void DefinitionNode::setVisibility(TranslationUnit::Visibility visibility) {
        _visibility = visibility;
    }

    TranslationUnit::Visibility DefinitionNode::visibility() const {
        return _visibility;
    }
}
