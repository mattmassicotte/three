#pragma once

#include "DataType.h"

#include <string>

namespace Three {
    class Annotation {
    public:
        virtual ~Annotation();
        virtual std::string CSourceString() const = 0;
        virtual void applyToDataType(DataType* dataType);

    public:
        std::string annotationName;
    };

    class NoReturnAnnotation : public Annotation {
    public:
        std::string CSourceString() const;
    };

    class InlineAnnotation : public Annotation {
    public:
        typedef enum {
            Never,
            Allowed,
            Always
        } Type;

    public:
        InlineAnnotation(InlineAnnotation::Type aType);

        std::string CSourceString() const;

    public:
        InlineAnnotation::Type type;
    };

    class CommonAnnotation : public Annotation {
    public:
        std::string expression;

        std::string CSourceString() const;
    };

    class CSourceAnnotation : public Annotation {
    public:
        typedef enum {
            NoTypedef
        } Type;

    public:
        CSourceAnnotation(CSourceAnnotation::Type aType);

        std::string CSourceString() const;
        void applyToDataType(DataType* dataType);

    public:
        CSourceAnnotation::Type type;
    };
}
