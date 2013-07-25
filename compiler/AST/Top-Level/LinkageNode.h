#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class LinkageNode : public ASTNode {
    public:
        static LinkageNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::string libraryName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _libraryName;
    };
}
