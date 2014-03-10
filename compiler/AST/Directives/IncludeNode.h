#pragma once

#include "ExternalSourceNode.h"

namespace Three {
    class IncludeNode : public ExternalSourceNode {
    public:
        static IncludeNode* parse(Parser& parser);

    public:
        std::string name() const;

        std::string headerName() const;
        bool relative() const;

    private:
        std::string _headerName;
    };
}
