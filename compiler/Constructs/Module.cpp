#include "Module.h"
#include "Function.h"

#include <assert.h>
#include <iostream>

namespace Language {
    Module* Module::createRootModule() {
        Module* module = new Module();

        module->addDataType(new DataType(DataType::Flavor::Scalar, "Void"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Int"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Char"));

        // function type: (Void; Void)
        DataType* type;

        type = new DataType(DataType::Flavor::Function, "(Void; Void)");
        type->addChild(TypeReference::ref(module, "Void", 0));
        type->setReturnType(TypeReference::ref(module, "Void", 0));
        module->addDataType(type);

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

    void Module::addDataType(DataType* type) {
        assert(type != NULL);
        // TODO: has to check if it already exists
        _dataTypes[type->name()] = type;
    }

    DataType* Module::dataTypeForName(const std::string& name) {
        DataType* type = _dataTypes[name];

        if (!type) {
            std::cout << "Module: asked for a type that does not exist '" << name << "'" << std::endl;
        }

        return _dataTypes[name];
    }
}
