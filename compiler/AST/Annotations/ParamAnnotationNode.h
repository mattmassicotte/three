#pragma once

#include "TextAnnotationNode.h"

namespace Three {
    class ParamAnnotationNode : public TextAnnotationNode {
    public:
        static ParamAnnotationNode* parse(NewParser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
