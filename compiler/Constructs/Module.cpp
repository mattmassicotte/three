#include "Module.h"
#include "../Parser.h"

#include <assert.h>
#include <fstream>
#include <iostream>

namespace Three {
    Module* Module::loadModule(Module* rootModule, std::string name, std::vector<std::string> searchPaths) {
        // add the default search location
        searchPaths.push_back("/usr/local/include/three/modules");

        // transform '.' -> '/'
        std::replace(name.begin(), name.end(), '.', '/');
        name += ".3"; // tack on our extension

        for (const std::string& s : searchPaths) {
            std::string fullPath = s + '/' + name;
            Module* m;

            m = Module::loadModuleAtPath(fullPath);
            if (m) {
                return m;
            }
        }

        return new Module();
    }

    Module* Module::loadModuleAtPath(const std::string& path) {
        std::cout << "Loading Module at: '" << path << "'" << std::endl;

        std::ifstream inputFile(path);

        Language::Lexer lexer(&inputFile);
        Language::Parser parser(&lexer);

        parser.setContext(Three::ParsingContext::translationUnitContext());

        RootNode* node = parser.rootASTNode();

        return parser.currentModule();
    }

    Module::Module() : _parentModule(NULL) {
    }

    Module::~Module() {
    }

    Module* Module::parent() const {
        return _parentModule;
    }

    void Module::setParent(Module* module) {
        assert(module);
        assert(module != this);

        _parentModule = module;
    }

    void Module::addCIncludePath(const std::string& value) {
        _cIncludePaths.push_back(value);
    }

    void Module::eachCIncludePath(std::function<void (const std::string&)> func) {
        for (const std::string& path : _cIncludePaths) {
            func(path);
        }
    }

    void Module::addLibraryDependency(const std::string& name) {
        _libraries.push_back(name);
    }

    Module* Module::importModule(const std::string& name, const std::vector<std::string>& searchPaths) {
        Module* m = Module::loadModule(this, name, searchPaths);

        if (!m) {
            std::cout << "Failed to load Module: '" << name << "'" << std::endl;

            return m;
        }

        _importedModules.push_back(m);

        return m;
    }

    void Module::addModule(const std::string& name, Module* module) {
        assert(module != _parentModule);

        // TODO: should make sure it isn't already defined or loaded

        _importedModules.push_back(module);
    }

    void Module::addFunction(const std::string& name, Function* func) {
        assert(func != NULL && "Function should not be null");
        assert((this->functionForName(name) == NULL) && "Function should not already exist");
        _functions[name] = func;
    }

    Function* Module::functionForName(const std::string& name) {
        Function* f;

        // recursively search through our imported modules
        for (Module* m : _importedModules) {
            f = m->functionForName(name);

            if (f) {
                return f;
            }
        }

        // now, check the parent
        if (_parentModule) {
            f = _parentModule->functionForName(name);
            if (f) {
                return f;
            }
        }

        return _functions[name];
    }

    void Module::addDataType(const std::string& name, DataType* type) {
        assert(type != NULL && "DataType should not be null");
        assert((this->dataTypeForName(name) == NULL) && "DataType should not already exist");

        // std::cout << "Defined data type: " << name << " for " << this << std::endl;
        _dataTypes[name] = type;
    }

    void Module::addDataType(DataType* type) {
        this->addDataType(type->name(), type);
    }

    void Module::aliasDataType(const std::string& name, DataType* type) {
        assert(type != NULL && "DataType should not be null");
        assert((this->dataTypeForName(name) == NULL) && "DataType should not already exist");

        _dataTypeAliases[name] = type;
    }

    DataType* Module::dataTypeForName(const std::string& name) {
        DataType* type;

        // first, recursively search through our imported modules
        for (Module* m : _importedModules) {
            type = m->dataTypeForName(name);

            if (type) {
                return type;
            }
        }

        if (_parentModule) {
            type = _parentModule->dataTypeForName(name);
            if (type) {
                return type;
            }
        }

        // std::cout << "checking data type: " << name << " for " << this << std::endl;

        // not found, so check our own types
        type = _dataTypes[name];
        if (type) {
            return type;
        }

        type = _dataTypeAliases[name];
        if (type) {
            return type;
        }

        return NULL;
    }
}
