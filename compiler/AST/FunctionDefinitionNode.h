#pragma once

#include "ASTNode.h"
#include "DataType.h"

namespace Language {
    class FunctionDefinitionNode : public ASTNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setFunctionName(const std::string& name);
        std::string functionName() const;
        std::vector<DataType> parameters() const;
        DataType returnType() const;

        virtual void renderCCode(std::stringstream& stream);

    private:
        std::string _functionName;

        std::vector<DataType> _parameterTypes;
        DataType              _returnType;
    };
}
