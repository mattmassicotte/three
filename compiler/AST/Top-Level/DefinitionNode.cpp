#include "DefinitionNode.h"
#include "TypeDefinitionNode.h"
#include "FunctionDeclarationNode.h"
#include "../FunctionDefinitionNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ASTNode* DefinitionNode::parse(Parser& parser, bool inModule) {
        assert(parser.next().type() == Token::Type::KeywordDef);

        if (parser.nextIf(":")) {
            if (parser.nextIf("type")) {
                return TypeDefinitionNode::parse(parser);
            }

            assert(0 && "Unrecognized def qualifier");
        }

        // change the meaning of "def" keyword
        if (inModule) {
            return FunctionDeclarationNode::parse(parser);
        }

        return FunctionDefinitionNode::parse(parser);
    }

    DefinitionNode::DefinitionNode() : _visibility(TranslationUnit::Visibility::External) {
    }

    TranslationUnit::Visibility DefinitionNode::visibility() const {
        return _visibility;
    }
}
