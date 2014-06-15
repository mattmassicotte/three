#pragma once

#include "AnnotationNode.h"

namespace Three {
    class ReadAnnotationNode : public AnnotationNode {
    public:
        static ReadAnnotationNode* parse(Parser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
