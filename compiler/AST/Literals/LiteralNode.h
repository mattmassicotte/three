#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"

namespace Three {
    class LiteralNode : public ASTNode {
    public:
        static LiteralNode* parse(NewParser& parser);

    public:
        std::string name() const { return "tmp"; } // to be removed eventually

        NewDataType dataType() const;
        void setDataType(const NewDataType& type);

    private:
        NewDataType _dataType;
    };
}
