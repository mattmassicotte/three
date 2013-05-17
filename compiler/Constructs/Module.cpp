#include "Module.h"
#include "Function.h"

#include <assert.h>

namespace Language {
    Module* Module::createRootModule() {
        Module* module = new Module();

        module->addDataStructure("Void", new DataStructure("Void"));
        module->addDataStructure("Int",  new DataStructure("Int"));
        module->addDataStructure("Char", new DataStructure("Char"));

        return module;
    }

    Module* Module::createModule(Module* rootModule, const std::string& path, std::vector<std::string> basePaths) {
        Module* module = new Module();

        if (path == "C.stdio") {
            module->setCIncludePath("stdio.h");

            Function* func = new Function();
            func->setName("printf");
            func->setReturnType(DataType(rootModule->dataStructureForName("Int"), 1));

            func->addParameter("format", DataType(rootModule->dataStructureForName("Char"), 1));

            module->addFunction("printf", func);
        }

        return module;
    }

    Module::Module() {
    }

    Module::~Module() {
        for (Module* child : _childModules) {
            delete child;
        }

        for (auto& kv : _functions) {
            delete kv.second;
        }

        for (auto& kv : _dataStructures) {
            delete kv.second;
        }
    }

    void Module::setCIncludePath(const std::string& value) {
        _cIncludePath = value;
    }

    std::string Module::cIncludePath() const {
        return _cIncludePath;
    }

    void Module::addFunction(const std::string& name, Function* func) {
        assert(func != NULL);
        _functions[name] = func;
    }

    Function* Module::functionForName(const std::string& name) {
        for (Module* child : _childModules) {
            Function* func = child->functionForName(name);
            if (func) {
                return func;
            }
        }

        return _functions[name];
    }

    void Module::addDataStructure(const std::string& name, DataStructure* type) {
        assert(type != NULL);
        _dataStructures[name] = type;
    }

    DataStructure* Module::dataStructureForName(const std::string& name) {
        return _dataStructures[name];
    }

    void Module::addChild(Module* child) {
        _childModules.push_back(child);
    }
}
