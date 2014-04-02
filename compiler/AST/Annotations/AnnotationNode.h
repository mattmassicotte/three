#pragma once

#include "../ASTNode.h"

namespace Three {
    class AnnotationNode : public ASTNode {
    public:
        static AnnotationNode* parse(NewParser& parser);

    public:
        virtual ~AnnotationNode();

        virtual std::string name() const;
    };
}
