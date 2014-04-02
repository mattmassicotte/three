#pragma once

#include "TextAnnotationNode.h"

namespace Three {
    class SummaryAnnotationNode : public TextAnnotationNode {
    public:
        static SummaryAnnotationNode* parse(NewParser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
