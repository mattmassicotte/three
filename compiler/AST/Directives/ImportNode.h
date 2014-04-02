#pragma once

#include "ExternalSourceNode.h"

namespace Three {
    class ImportNode : public ExternalSourceNode {
    public:
        static ImportNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;
        std::string str() const;

        std::string headerName() const;
        bool relative() const;
        void setPath(const std::string& value);
        std::string path() const;
        std::string resolvedFilePath() const;

    private:
        std::string _path;
    };
}
