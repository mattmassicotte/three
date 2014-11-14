#pragma once

#include "../OperatorNode.h"

namespace Three {
    class CallableOperatorNode : public OperatorNode {
    public:
        static bool parseArguments(Parser& parser, CallableOperatorNode* node);

    public:
        CallableOperatorNode();
        virtual ~CallableOperatorNode();

        DataType dataType() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        ASTNode* receiver() const;
        void setReceiver(ASTNode* node);
        DataType receiverDataType() const;

    private:
        ASTNode* _receiver;
    };
}
