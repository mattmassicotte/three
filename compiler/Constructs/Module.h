#pragma once

#include "../Helpers/TreeStructure.hpp"
#include "Function.h"
#include "DataType.h"

#include <string>
#include <vector>
#include <map>

namespace Language {
    class Module : public TreeStructure<Module*> {
    public:
        static Module* createRootModule();
        static Module* createModule(Module* rootModule, const std::string& path, std::vector<std::string> basePaths);

    public:
        Module();
        virtual ~Module();

        void setCIncludePath(const std::string& value);
        std::string cIncludePath() const;

        void addFunction(const std::string& name, Function* func);
        Function* functionForName(const std::string& name);

        void addDataType(const std::string& name, DataType* type);
        DataType* dataTypeForName(const std::string& name);

    private:
        std::string _cIncludePath;

        std::map<std::string, DataType*> _dataTypes;
        std::map<std::string, Function*> _functions;
        
    };
}
