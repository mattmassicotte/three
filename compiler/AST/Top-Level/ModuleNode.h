#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Module.h"

using namespace Language;

namespace Three {
    class ModuleNode : public ASTNode {
    public:
        static ModuleNode* parse(Parser& parser);
        static ASTNode*    parseSubmoduleStatement(Parser& parser);

    public:
        virtual std::string name() const;
        std::string moduleName() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _name;
        Three::Module* _module;
    };
}
