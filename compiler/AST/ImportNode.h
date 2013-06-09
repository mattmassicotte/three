#pragma once

#include "ASTNode.h"
#include "../Constructs/Module.h"

namespace Language {
    class ImportNode : public ASTNode {
    public:
        static ImportNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setPath(const std::string& value);
        std::string path() const;
        Module* module() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _path;
        Module*     _module;
    };
}
