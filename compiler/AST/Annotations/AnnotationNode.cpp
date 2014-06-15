#include "Annotations.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    AnnotationNode* AnnotationNode::parse(Parser& parser) {
        switch (parser.helper()->peek().type()) {
            case Token::Type::AnnotationAccess:
            case Token::Type::AnnotationVolatile:
            case Token::Type::AnnotationAlias:
            case Token::Type::AnnotationConst:
            case Token::Type::AnnotationRestrict:
                assert(0 && "Message: Annotation must be used as a type qualifier");
                break;
            case Token::Type::AnnotationBrief:
                return BriefAnnotationNode::parse(parser);
            case Token::Type::AnnotationSummary:
                return SummaryAnnotationNode::parse(parser);
            case Token::Type::AnnotationParam:
                return ParamAnnotationNode::parse(parser);
            case Token::Type::AnnotationRead:
                return ReadAnnotationNode::parse(parser);
            case Token::Type::AnnotationWrite:
            case Token::Type::AnnotationAssert:
            case Token::Type::AnnotationSentinel:
            case Token::Type::AnnotationChecked:
            case Token::Type::AnnotationNull:
            case Token::Type::AnnotationSuccess:
            case Token::Type::AnnotationGlobal:
            case Token::Type::AnnotationThread:
            case Token::Type::AnnotationIO:
            case Token::Type::AnnotationRegister:
            case Token::Type::AnnotationMemory:
            case Token::Type::AnnotationFlow:
            case Token::Type::AnnotationAvailable:
                assert(0 && "Unhandled annotation");
                break;
            case Token::Type::AnnotationNoreturn:
                return NoreturnAnnotationNode::parse(parser);

            case Token::Type::AnnotationReturn:
                return ReturnAnnotationNode::parse(parser);
            case Token::Type::AnnotationCommon:
            case Token::Type::AnnotationUncommon:
            case Token::Type::AnnotationOptimize:
            case Token::Type::AnnotationInline:
            case Token::Type::AnnotationPrefetch:
            case Token::Type::AnnotationPure:
            case Token::Type::AnnotationThrows:
                assert(0 && "Unhandled annotation");
                break;
            default:
                break;
        }

        assert(0 && "Unrecognized annotation");

        return nullptr;
    }

    AnnotationNode::~AnnotationNode() {
    }

    std::string AnnotationNode::name() const {
        return "Annotation";
    }
}
