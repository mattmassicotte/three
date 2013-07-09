#include "Annotation.h"

namespace Language {
    Annotation::~Annotation() {
    }

    std::string NoReturnAnnotation::CSourceString() const {
        return "THREE_ATTR_NORETURN";
    }

    InlineAnnotation::InlineAnnotation(InlineAnnotation::Type aType) : type(aType) {
    }

    std::string InlineAnnotation::CSourceString() const {
        switch (this->type) {
            case InlineAnnotation::Never:
                return "THREE_ATTR_INLINE_NEVER";
            case InlineAnnotation::Always:
                return "THREE_ATTR_INLINE_ALWAYS";
            default:
                break;
        }

        return "";
    }

    std::string CommonAnnotation::CSourceString() const {
        return "";
    }
}
