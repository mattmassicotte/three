#include "LiteralNode.h"
#include "RealLiteralNode.h"
#include "CharacterLiteralNode.h"
#include "BooleanLiteralNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    LiteralNode* LiteralNode::parse(Parser& parser) {
        switch (parser.helper()->peek().type()) {
            case Token::Type::LiteralReal:
                return RealLiteralNode::parse(parser);
            case Token::Type::LiteralScientificNotation:
                break;
            case Token::Type::LiteralCharacter:
                return CharacterLiteralNode::parse(parser);
            case Token::Type::LiteralTrue:
            case Token::Type::LiteralFalse:
                return BooleanLiteralNode::parse(parser);
            default:
                break;
        }

        assert(0 && "Unimplemented");

        return nullptr;
    }

    NewDataType LiteralNode::dataType() const {
        return _dataType;
    }

    void LiteralNode::setDataType(const NewDataType& type) {
        _dataType = type;
    }
}
