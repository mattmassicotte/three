#pragma once

#include "Constructs/Module.h"

#include <string>
#include <map>
#include <functional>

namespace Three {
    class CSourceIndexer {
    public:
        static std::vector<std::string>* defaultCIncludePaths();
        static std::string resolveCHeaderPath(const std::string& partialPath);
        static std::vector<std::string> defaultCCompilerArguments();

    public:
        CSourceIndexer();
        virtual ~CSourceIndexer();

        bool indexFileAtPath(const std::string& path);
        void addFunction(const std::string& name);
        void addType(const std::string& name, DataType::Flavor flavor);
        void addVariable(const std::string& name);
        void addConstant(const std::string& name);

        Module* module() const;

    public:
        bool verbose;

    private:
        Module* _module;
    };
}
