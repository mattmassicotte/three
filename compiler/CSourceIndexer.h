#pragma once

#include <string>

namespace Three {
    class CSourceIndexer {
    public:
        CSourceIndexer();
        virtual ~CSourceIndexer();

        bool indexFileAtPath(const char* path);
    };
}
