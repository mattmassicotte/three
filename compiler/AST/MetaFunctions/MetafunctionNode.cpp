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
                return AlignofNode::parse(parser);
            case Token::Type::MetafunctionOffsetof:
                return OffsetofNode::parse(parser);
            case Token::Type::MetafunctionABIof:
                return ABIofNode::parse(parser);
            case Token::Type::MetafunctionTypeof:
                return TypeofNode::parse(parser);
            case Token::Type::MetafunctionNameof:
                return NameofNode::parse(parser);
            case Token::Type::MetafunctionCardinalityof:
                return CardinalityofNode::parse(parser);
            case Token::Type::MetafunctionNextarg:
                return NextargNode::parse(parser);
            case Token::Type::MetafunctionContainerof:
                return ContainerofNode::parse(parser);
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
