#pragma once

#include "AnnotationNode.h"

namespace Three {
    class TextAnnotationNode : public AnnotationNode {
    public:
        virtual std::string nodeName() const;

        std::string text() const;

    protected:
        bool parseText(NewParser& parser);

    private:
        std::string _text;
    };
}
