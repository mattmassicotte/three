#include "ParseContext.h"
#include "compiler/Messages/Message.h"
#include "compiler/AST/RootNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/constructs/Scope.h"
#include "compiler/constructs/QualifiedName.h"
#include "compiler/constructs/Variable.h"
#include "compiler/CSourceIndexer.h"
#include "compiler/CSourceEmitter.h"
#include "compiler/Parser/Parser.h"

#include <fstream>

namespace Three {
    ParseContext::ParseContext() : skipIncludes(false), skipImports(false) {
        _rootNode = new RootNode();
        _rootScope = new Scope();
        _currentScope = _rootScope;
        _visibility = TranslationUnit::Visibility::Default;

        _dataTypeMap["Void"]    = DataType(DataType::Kind::Void);
        _dataTypeMap["Bool"]    = DataType(DataType::Kind::Boolean);
        _dataTypeMap["Int"]     = DataType(DataType::Kind::Integer);
        _dataTypeMap["Natural"] = DataType(DataType::Kind::Natural);
        _dataTypeMap["Float"]   = DataType(DataType::Kind::Float);
        _dataTypeMap["Real"]    = DataType(DataType::Kind::Real);
        _dataTypeMap["Char"]    = DataType(DataType::Kind::Character);
        _dataTypeMap["Vararg"]  = DataType(DataType::Kind::Vararg);
    }

    ParseContext::~ParseContext() {
        delete _rootNode;
        delete _currentScope;

        this->clearMessages();

        for (ParseContext* subcontext : _importedContexts) {
            delete subcontext;
        }
    }

    RootNode* ParseContext::rootNode() const {
        return _rootNode;
    }

    uint32_t ParseContext::messageCount() const {
        return _messages.size();
    }

    Message* ParseContext::messageAtIndex(uint32_t i) const {
        return _messages.at(i);
    }

    void ParseContext::addMessage(Message* message) {
        _messages.push_back(message);
    }

    void ParseContext::clearMessages() {
        for (Message* message : _messages) {
            delete message;
        }

        _messages.clear();
    }

    bool ParseContext::displayMessages() {
        // handle messages
        for (int i = 0; i < this->messageCount(); ++i) {
            std::cout << this->messageAtIndex(i)->str() << std::endl;
        }

        return this->messageCount() > 0;
    }

    bool ParseContext::import(const std::string& name) {
        if (skipImports) {
            return true;
        }

        // resolve name
        std::string path = this->resolveImportPath(name);
        if (this->hasImported(path + ".h")) {
            std::cout << "Duplicate import '" << name << "'" << std::endl;
            return true;
        }

        std::unique_ptr<ParseContext> importedContext(new ParseContext());

        // if we find a .3 file, but not a .h file, we have to compile it first
        if (!Parser::parse(std::string(path + ".3").c_str(), importedContext.get())) {
            std::cout << "Unable to parse import" << std::endl;
            return false;
        }

        if (!CSourceEmitter::createSourcesAtPath(*importedContext.get(), path)) {
            std::cout << "Unable to emit C sources for import" << std::endl;
            return false;
        }

        // make things in the current context visible
        _importedContexts.push_back(importedContext.release());

        // need to create function variables for non-C functions
        // for (std::pair<std::string, DataType> fnPair: importedContext->_functions) {
        //     if (fnPair.second.kind() == DataType::Kind::CFunction) {
        //         continue;
        //     }
        // 
        //     this->scope()->defineVariableTypeForName(fnPair.second, fnPair.first);
        // }

        // CSourceIndexer indexer;
        // 
        // if (!indexer.indexFileAtPath(path + ".h", this)) {
        //     std::cout << "Unable to index import header" << std::endl;
        //     return false;
        // }

        _importedPaths.push_back(path);

        return true;
    }

    void ParseContext::addImportSearchPath(const std::string& name) {
        _importSearchPaths.push_back(name);
    }

    std::string ParseContext::resolveImportPath(const std::string& name) {
        for (const std::string& path : _importSearchPaths) {
            std::string fullPath = path + "/" + name;

            // constructing an ifstream checks if we can open the file for reading
            std::ifstream file(fullPath + ".3");

            if (file) {
                return fullPath;
            }
        }

        return "";
    }

    std::vector<std::string> ParseContext::importedPaths() const {
        return _importedPaths;
    }

    bool ParseContext::hasImported(const std::string& path) const {
        return std::find(_importedPaths.cbegin(), _importedPaths.cend(), path) != _importedPaths.cend();
    }

    void ParseContext::setVisibility(TranslationUnit::Visibility visibility) {
        _visibility = visibility;
    }

    TranslationUnit::Visibility ParseContext::visibility() const {
        return _visibility;
    }

    DataType ParseContext::dataTypeForName(const QualifiedName& name) const {
        // first, search for given name
        DataType type = this->dataTypeForExactName(name.to_s());
        if (type.defined()) {
            return type;
        }

        // ok, not found. Try applying this scope's namespace
        if (this->scope()->fullNamespace().numberOfComponents() == 0) {
            return DataType();
        }

        QualifiedName namespacedName(name);

        namespacedName.prependName(this->scope()->fullNamespace());

        return this->dataTypeForExactName(namespacedName.to_s());
    }

    DataType ParseContext::dataTypeForExactName(const std::string& name) const {
        auto it = _dataTypeMap.find(name);

        if (it != _dataTypeMap.cend()) {
            return it->second;
        }

        // search imported context
        for (ParseContext* subcontext : _importedContexts) {
            DataType type = subcontext->dataTypeForName(name);

            if (type.defined()) {
                return type;
            }
        }

        return DataType();
    }

    bool ParseContext::identifierAvailableForDefinition(const std::string& name) const {
        if (this->definesTypeWithName(name)) {
            return false;
        }

        // also have to restrict a few keywords
        if (name == "cas") {
            return false;
        }

        for (ParseContext* subcontext : _importedContexts) {
            if (!subcontext->identifierAvailableForDefinition(name)) {
                return false;
            }
        }

        return true;
    }

    DataType::Kind ParseContext::typeKindWithName(const std::string& name) const {
        return this->dataTypeForName(name).kind();
    }

    bool ParseContext::definesTypeWithName(const QualifiedName& name) const {
        // first, search for given name
        if (this->definesTypeWithName(name.to_s())) {
            return true;
        }

        // ok, not found. Try applying this scope's namespace
        if (this->scope()->fullNamespace().numberOfComponents() == 0) {
            return false;
        }

        QualifiedName namespacedName(name);

        namespacedName.prependName(this->scope()->fullNamespace());

        return this->definesTypeWithName(namespacedName.to_s());
    }

    bool ParseContext::definesTypeWithName(const std::string& name) const {
        return this->typeKindWithName(name) != DataType::Kind::Undefined;
    }

    bool ParseContext::defineTypeForName(DataType type, const std::string& name) {
        if (this->definesTypeWithName(name)) {
            return false;
        }

        _dataTypeMap[name] = type;

        return true;
    }

    bool ParseContext::redefineTypeForName(DataType type, const std::string& name) {
        if (!this->definesTypeWithName(name)) {
            return false;
        }

        _dataTypeMap[name] = type;

        return true;
    }

    DataType ParseContext::functionForName(const std::string& name) const {
        auto it = _functions.find(name);

        if (it != _functions.cend()) {
            return it->second;
        }

        for (ParseContext* subcontext : _importedContexts) {
            DataType fnType = subcontext->functionForName(name);

            if (fnType.defined()) {
                return fnType;
            }
        }

        return DataType();
    }

    bool ParseContext::defineFunctionForName(const DataType& type, const std::string& name) {
        assert((type.kind() == DataType::Kind::Function) || (type.kind() == DataType::Kind::CFunction));

        if (this->functionForName(name).kind() != DataType::Kind::Undefined) {
            return false;
        }

        // now, define this function as a "variable" in the global scope
        if (!this->defineVariableTypeForName(type, name, false)) {
            return false;
        }

        _functions[name] = type;

        return true;
    }

    Variable* ParseContext::variableForName(const QualifiedName& name) const {
        // first, search for given name
        Variable* v = this->variableForExactName(name.to_s());
        if (v) {
            return v;
        }

        // ok, not found. Try applying this scope's namespace
        if (this->scope()->fullNamespace().numberOfComponents() == 0) {
            return nullptr;
        }

        QualifiedName namespacedName(name);

        namespacedName.prependName(this->scope()->fullNamespace());

        return this->variableForExactName(namespacedName.to_s());
    }

    Variable* ParseContext::variableForExactName(const std::string& name) const {
        auto it = _variables.find(name);

        if (it != _variables.cend()) {
            return it->second;
        }

        for (ParseContext* subcontext : _importedContexts) {
            Variable* v = subcontext->variableForName(name);

            if (v) {
                return v;
            }
        }

        return this->scope()->variableForName(name);
    }

    bool ParseContext::defineVariable(Variable* variable, bool scoped) {
        if (scoped) {
            return this->scope()->defineVariable(variable);
        }

        if (this->variableForName(QualifiedName(variable->name))) {
            return false;
        }

        _variables[variable->name] = variable;

        return true;
    }

    bool ParseContext::defineVariableTypeForName(const DataType& type, const std::string& name, bool scoped) {
        Variable* variable = new Variable();

        variable->name = name;
        variable->type = type;

        return this->defineVariable(variable, scoped);
    }

    Scope* ParseContext::scope() const {
        return _currentScope;
    }

    void ParseContext::pushScope() {
        Scope* scope = new Scope();

        scope->setParent(_currentScope);

        _currentScope = scope;
    }

    void ParseContext::popScope() {
        assert(_currentScope != _rootScope);

        Scope* old = _currentScope;

        assert(old->parent());
        _currentScope = old->parent();

        delete old;
    }

    Scope* ParseContext::rootScope() const {
        return _rootScope;
    }

    void ParseContext::postProcessAST() {
        if (!_rootNode) {
            return;
        }
    }
}
