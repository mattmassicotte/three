#pragma once

#include <string>

namespace Language {
    class Annotation {
    public:
        virtual ~Annotation();
        virtual std::string CSourceString() const = 0;
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
        InlineAnnotation(InlineAnnotation::Type type);

        std::string CSourceString() const;

    public:
        InlineAnnotation::Type type;
    };

    class CommonAnnotation : public Annotation {
    public:
        std::string expression;

        std::string CSourceString() const;
    };
}
