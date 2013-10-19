#pragma once

#include "Module.h"
#include "DataType.h"

#include <string>
#include <vector>
#include <map>

namespace Three {
    class TranslationUnit {
    public:
        typedef enum {
            Default,
            None,
            Internal,
            External
        } Visibility;

    public:
        static TranslationUnit* createTranslationUnit();

    public:
        TranslationUnit();
        virtual ~TranslationUnit();

        void addFunction(const std::string& name, Function* func);
        Function* functionForName(const std::string& name);

        void addDataType(DataType* type);
        DataType* dataTypeForName(const std::string& name);
        void aliasDataType(const std::string& name, DataType* type);

        Three::Module* module() const;

    private:
        Three::Module* _module;
    };
}
