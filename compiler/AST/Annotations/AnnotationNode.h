#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Annotation.h"

namespace Three {
    class AnnotationNode : public ASTNode {
    public:
        static AnnotationNode* parse(Parser& parser);

    public:
        virtual ~AnnotationNode();

        virtual std::string name() const;

        Annotation* annotation() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Annotation* _annotation;
    };
}
