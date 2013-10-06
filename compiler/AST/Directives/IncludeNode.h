#pragma once

#include "../ASTNode.h"

namespace Three {
    class IncludeNode : public ASTNode {
    public:
        static IncludeNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string headerName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _headerName;
    };
}
