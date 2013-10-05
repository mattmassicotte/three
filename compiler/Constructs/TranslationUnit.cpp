#include "TranslationUnit.h"
#include "Function.h"

#include <assert.h>
#include <iostream>

namespace Language {
    TranslationUnit* TranslationUnit::createTranslationUnit() {
        TranslationUnit* translationUnit = new TranslationUnit();
        Three::Module* module = new Three::Module();

        translationUnit->_module = module;

        // if (path == "C.stdio") {
        //     module->setCIncludePath("stdio.h");
        // 
        //     Function* func = new Function();
        //     func->setName("printf");
        //     func->setReturnType(TypeReference::ref(rootTranslationUnit, "Int", 0));
        //     func->addParameter("format", TypeReference::ref(rootTranslationUnit, "Char", 1));
        // 
        //     module->addFunction("printf", func);
        // }

        module->addDataType(new DataType(DataType::Flavor::Scalar, "Void"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Int"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Int:64"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Natural"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Natural:32"));
        module->addDataType(new DataType(DataType::Flavor::Scalar, "Char"));

        // function type: (Void; Void)
        DataType* type;

        type = new DataType(DataType::Flavor::Function, "(Void; Void)");
        type->addChild(TypeReference::ref(module, "Void", 0), "arg");
        type->setReturnType(TypeReference::ref(module, "Void", 0));
        module->addDataType(type);

        return translationUnit;
    }

    TranslationUnit::TranslationUnit() {
    }

    TranslationUnit::~TranslationUnit() {
    }

    void TranslationUnit::addFunction(const std::string& name, Function* func) {
        this->_module->addFunction(name, func);
    }

    Function* TranslationUnit::functionForName(const std::string& name) {
        Function* f = this->_module->functionForName(name);

        if (!f) {
            std::cout << "TranslationUnit: function does not exist '" << name << "'" << std::endl;
        }

        return f;
    }

    void TranslationUnit::addDataType(DataType* type) {
        this->_module->addDataType(type);
    }

    DataType* TranslationUnit::dataTypeForName(const std::string& name) {
        DataType* type = this->_module->dataTypeForName(name);

        if (!type) {
            std::cout << "TranslationUnit: type that does not exist '" << name << "'" << std::endl;
        }

        return type;
    }

    Three::Module* TranslationUnit::module() const {
        return _module;
    }
}
