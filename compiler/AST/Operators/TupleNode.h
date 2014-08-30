#pragma once

#include "../ASTNode.h"

namespace Three {
    class TupleNode : public ASTNode {
    public:
        static TupleNode* parse(Parser& parser, ASTNode* firstElement, bool fullExpressions, const NewDataType* expectedType = nullptr);

    public:
        NewDataType dataType() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        std::string str() const;

    private:
        NewDataType _type;
    };
}
