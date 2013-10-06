#pragma once

#include "Function.h"
#include "DataType.h"

#include <functional>
#include <map>
#include <vector>

namespace Three {
    class Module {
    public:
        typedef enum {
            None,
            Internal,
            External
        } Visibility;

    public:
        static Module* loadModule(Module* rootModule, std::string name, std::vector<std::string> searchPaths);
        static Module* loadModuleAtPath(const std::string& path);

    public:
        Module();
        virtual ~Module();

        Module* parent() const;
        void setParent(Module* module);

        void addCIncludePath(const std::string& value);
        void eachCIncludePath(std::function<void (const std::string&)> func) const;
        void addLibraryDependency(const std::string& name);

        Module* importModule(const std::string& name, const std::vector<std::string>& searchPaths);
        void    addModule(const std::string& name, Module* module);

        void addFunction(const std::string& name, Function* func);
        Function* functionForName(const std::string& name);

        void addDataType(const std::string& name, DataType* type);
        void addDataType(DataType* type);
        void aliasDataType(const std::string& name, DataType* type);
        DataType* dataTypeForName(const std::string& name);
        void removeDataTypeForName(const std::string& name);

        void addConstant(const std::string& name, const std::string& value);
        std::string constantForName(const std::string& name);
        bool definesConstant(const std::string& name);

    private:
        std::vector<Module*> _importedModules;
        Module* _parentModule;

        std::vector<std::string> _cIncludePaths;
        std::vector<std::string> _libraries;

        std::map<std::string, DataType*> _dataTypes;
        std::map<std::string, DataType*> _dataTypeAliases;
        std::map<std::string, Function*> _functions;
        std::map<std::string, std::string> _constants;
    };
}
