#pragma once

#include "../ASTNode.h"
#include "../../Constructs/DataType.h"

namespace Three {
    class FunctionCallNode : public ASTNode {
    public:
        static FunctionCallNode* parse(Parser& parser, const std::string& functionName, ASTNode* firstArgument);
        static FunctionCallNode* parseMethod(Parser& parser, ASTNode* target);

    public:
        virtual std::string name() const;
        std::string str() const;
        TypeReference functionType() const;

        bool isClosure() const;

        void setFunctionName(const std::string& name);
        std::string functionName() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _functionName;
        DataType*   _functionType;
    };
}