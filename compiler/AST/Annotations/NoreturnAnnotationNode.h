#pragma once

#include "AnnotationNode.h"

namespace Three {
    class NoreturnAnnotationNode : public AnnotationNode {
    public:
        static NoreturnAnnotationNode* parse(Parser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
