#pragma once

#include "../ASTNode.h"

namespace Three {
    class AnnotationNode : public ASTNode {
    public:
        static AnnotationNode* parse(Parser& parser);

    public:
        virtual ~AnnotationNode();

        virtual std::string name() const;
    };
}
