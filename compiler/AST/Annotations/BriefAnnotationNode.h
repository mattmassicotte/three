#pragma once

#include "TextAnnotationNode.h"

namespace Three {
    class BriefAnnotationNode : public TextAnnotationNode {
    public:
        static BriefAnnotationNode* parse(NewParser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
