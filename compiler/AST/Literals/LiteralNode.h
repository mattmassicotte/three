#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"

namespace Three {
    class LiteralNode : public ASTNode {
    public:
        static LiteralNode* parse(Parser& parser);

    public:
        std::string name() const { return "tmp"; } // to be removed eventually

        DataType dataType() const;
        void setDataType(const DataType& type);

    private:
        DataType _dataType;
    };
}
