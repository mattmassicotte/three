#pragma once

#include "Constructs/Module.h"

#include <string>
#include <map>
#include <functional>
#include <vector>

namespace Three {
    struct CompoundField {
        std::string name;
        std::string typeName;
        uint32_t indirection;
        std::vector<uint32_t> dimensions;
    };

    class CSourceIndexer {
    public:
        static std::vector<std::string>* defaultCIncludePaths();
        static std::string resolveCHeaderPath(const std::string& partialPath);
        static std::vector<std::string> defaultCCompilerArguments();

    public:
        CSourceIndexer();
        virtual ~CSourceIndexer();

        bool indexFileAtUnresolvedPath(const std::string& path);
        bool indexFileAtPath(const std::string& path);
        void addFunction(const std::string& name);
        void addType(const std::string& name, DataType::Flavor flavor);
        void addVariable(const std::string& name);
        void addConstant(const std::string& name);
        void addField(const CompoundField& field);

        Module* module() const;

    public:
        bool verbose;

    private:
        Module* _module;
        DataType* _currentCompoundType;
        std::vector<CompoundField> _fields;
    };
}
