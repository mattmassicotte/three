#include "MetafunctionNode.h"
#include "compiler/Parser/Parser.h"
#include "Metafunctions.h"

namespace Three {
    MetafunctionNode* MetafunctionNode::parse(Parser& parser) {
        switch (parser.helper()->peek().type()) {
            case Token::Type::MetafunctionSizeOf:
                return SizeofNode::parse(parser);
            case Token::Type::MetafunctionCast:
                return CastNode::parse(parser);
            case Token::Type::MetafunctionAlignof:
            case Token::Type::MetafunctionOffsetof:
            case Token::Type::MetafunctionTypeof:
            case Token::Type::MetafunctionNameof:
            case Token::Type::MetafunctionCardinalityof:
                break;
            case Token::Type::MetafunctionNextarg:
                return NextargNode::parse(parser);
            default:
                assert(0 && "Invalid metafunction token");
                break;
        }

        return nullptr;
    }

    std::string MetafunctionNode::nodeName() const {
        return "Metafunction";
    }

    void MetafunctionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
