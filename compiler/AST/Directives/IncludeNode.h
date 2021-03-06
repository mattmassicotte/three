#pragma once

#include "ExternalSourceNode.h"

namespace Three {
    class IncludeNode : public ExternalSourceNode {
    public:
        static IncludeNode* parse(Parser& parser);
        static std::string parseSearchPathArgument(Parser& parser);

    public:
        std::string name() const;

        std::string headerName() const;
        bool relative() const;

        bool useSearchPaths() const;

    private:
        std::string _headerName;
        bool _useSearchPaths;
    };
}
