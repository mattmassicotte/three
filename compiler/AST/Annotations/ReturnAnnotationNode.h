#pragma once

#include "TextAnnotationNode.h"

namespace Three {
    class ReturnAnnotationNode : public TextAnnotationNode {
    public:
        static ReturnAnnotationNode* parse(Parser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
