#pragma once

#include "../ASTNode.h"

namespace Language {
    class BarrierNode : public ASTNode {
    public:
        static BarrierNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string type() const;

    protected:
        std::string c11MemoryOrderString() const;

    public:
        void codeGenCSource(CSourceContext& context);

    private:
        std::string _type;
    };
}
