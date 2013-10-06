#include "Annotation.h"

#include <assert.h>

namespace Three {
    Annotation::~Annotation() {
    }

    void Annotation::applyToDataType(DataType* dataType) {
        assert(0 && "Annotation cannot be applied to DataType");
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

    CSourceAnnotation::CSourceAnnotation(CSourceAnnotation::Type aType) : type(aType) {
    } 

    std::string CSourceAnnotation::CSourceString() const {
        return "";
    }

    void CSourceAnnotation::applyToDataType(DataType* dataType) {
        // TODO: this needs to select on the type
        assert(dataType->flavor() == DataType::Flavor::Structure);

        dataType->setCSourcePrependStructKeyword(true);
    }
}
