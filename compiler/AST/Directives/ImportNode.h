#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TranslationUnit.h"

namespace Three {
    class ImportNode : public ASTNode {
    public:
        static ImportNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setPath(const std::string& value);
        std::string path() const;
        std::string resolvedFilePath() const;
        Three::Module* module() const;

        void codeGen(CSourceContext& context);

    private:
        std::string    _path;
        Three::Module* _module;
        TranslationUnit::Visibility _visibility;
    };
}
