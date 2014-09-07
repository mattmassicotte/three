#pragma once

#include "compiler/Parser/ParseContext.h"

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
        static bool resolvePathAndIndex(const std::string& path, bool useSearchPaths, ParseContext* context);

    public:
        static std::vector<std::string>* defaultCIncludePaths();
        static std::string resolveCHeaderPath(const std::string& partialPath);
        static std::vector<std::string> defaultCCompilerArguments();

    public:
        CSourceIndexer();
        virtual ~CSourceIndexer();

        bool indexFileAtUnresolvedPath(const std::string& path, ParseContext* context);
        bool indexFileAtPath(const std::string& path, ParseContext* context);

        NewDataType dataTypeForCType(void* typePtr);

        void addFunction(const std::string& name, const void* declInfoPtr);
        void addType(const std::string& name, const void* declInfoPtr);
        void addVariable(const std::string& name);
        void addConstant(const std::string& name);
        void addField(const CompoundField& field);

        void addMacro(const std::string& name);

    public:
        bool verbose;

    private:
        ParseContext* _context;
        // DataType* _currentCompoundType;
        std::vector<CompoundField> _fields;
    };
}
