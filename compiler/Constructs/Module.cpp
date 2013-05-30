#include "Module.h"
#include "Function.h"

#include <assert.h>

namespace Language {
    Module* Module::createRootModule() {
        Module* module = new Module();

        module->addDataType("Void", new DataType(DataType::Flavor::Scalar, "Void"));
        module->addDataType("Int",  new DataType(DataType::Flavor::Scalar, "Int"));
        module->addDataType("Char", new DataType(DataType::Flavor::Scalar, "Char"));

        return module;
    }

    Module* Module::createModule(Module* rootModule, const std::string& path, std::vector<std::string> basePaths) {
        Module* module = new Module();

        if (path == "C.stdio") {
            module->setCIncludePath("stdio.h");

            Function* func = new Function();
            func->setName("printf");
            func->setReturnType(TypeReference::ref(rootModule, "Int", 0));
            func->addParameter("format", TypeReference::ref(rootModule, "Char", 1));

            module->addFunction("printf", func);
        }

        return module;
    }

    Module::Module() {
    }

    Module::~Module() {
        for (auto& kv : _functions) {
            delete kv.second;
        }

        for (auto& kv : _dataTypes) {
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
        for (uint32_t i = 0; i < this->childCount(); ++i) {
            Function* func = this->childAtIndex(i)->functionForName(name);
            if (func) {
                return func;
            }
        }

        return _functions[name];
    }

    void Module::addDataType(const std::string& name, DataType* type) {
        assert(type != NULL);
        _dataTypes[name] = type;
    }

    DataType* Module::dataTypeForName(const std::string& name) {
        return _dataTypes[name];
    }
}
