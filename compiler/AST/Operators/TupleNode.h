#pragma once

#include "../ASTNode.h"

namespace Three {
    class TupleNode : public ASTNode {
    public:
        static TupleNode* parse(Parser& parser, ASTNode* firstElement, bool fullExpressions, const DataType* expectedType = nullptr);

    public:
        DataType dataType() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        std::string str() const;

    private:
        DataType _type;
    };
}
