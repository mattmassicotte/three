#pragma once

#include <string>

namespace Three {
    class ParseContext;
}

namespace Three {
    class CSourceEmitter {
    public:
        static bool createSourcesAtPath(const class ParseContext& context, const std::string& outputBasePath);
        static std::string pathWithoutExtension(const std::string& path);
    };
}
