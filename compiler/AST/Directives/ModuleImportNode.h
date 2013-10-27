#pragma once

#include "../ASTNode.h"

namespace Three {
    class ModuleImportNode : public ASTNode {
    public:
        static ModuleImportNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void setPath(const std::string& value);
        std::string path() const;

    private:
        std::string    _path;
    };
}
