#pragma once

#include "Function.h"
#include "DataStructure.h"

#include <string>
#include <vector>
#include <map>

namespace Language {
    class Module {
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

        void addDataStructure(const std::string& name, DataStructure* type);
        DataStructure* dataStructureForName(const std::string& name);

        void addChild(Module* child);

    private:
        std::vector<Module*> _childModules;

        std::string _cIncludePath;

        std::map<std::string, DataStructure*> _dataStructures;
        std::map<std::string, Function*>      _functions;
        
    };
}
