#pragma once

#include "../OperatorNode.h"

namespace Three {
    class CallableOperatorNode : public OperatorNode {
    public:
        static bool parseArguments(NewParser& parser, CallableOperatorNode* node);

    public:
        virtual ~CallableOperatorNode();

        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        ASTNode* receiver() const;
        void setReceiver(ASTNode* node);
        NewDataType receiverDataType() const;

    private:
        ASTNode* _receiver;
    };
}
