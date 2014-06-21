#include "ParseContext.h"
#include "compiler/Messages/Message.h"
#include "compiler/AST/RootNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/constructs/NewScope.h"
#include "compiler/CSourceIndexer.h"
#include "compiler/CSourceEmitter.h"
#include "compiler/Parser/Parser.h"

#include <fstream>

namespace Three {
    ParseContext::ParseContext() : skipIncludes(false), skipImports(false) {
        _rootNode = new RootNode();
        _currentScope = new NewScope();
        _visibility = TranslationUnit::Visibility::Default;

        _dataTypeMap["Void"]    = NewDataType(NewDataType::Kind::Void);
        _dataTypeMap["Bool"]    = NewDataType(NewDataType::Kind::Boolean);
        _dataTypeMap["Int"]     = NewDataType(NewDataType::Kind::Integer);
        _dataTypeMap["Natural"] = NewDataType(NewDataType::Kind::Natural);
        _dataTypeMap["Float"]   = NewDataType(NewDataType::Kind::Float);
        _dataTypeMap["Real"]    = NewDataType(NewDataType::Kind::Real);
        _dataTypeMap["Char"]    = NewDataType(NewDataType::Kind::Character);
        _dataTypeMap["Vararg"]  = NewDataType(NewDataType::Kind::Vararg);
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

        ParseContext* importedContext = new ParseContext();

        // if we find a .3 file, but not a .h file, we have to compile it first
        if (!Parser::parse(std::string(path + ".3").c_str(), importedContext)) {
            std::cout << "Unable to parse import" << std::endl;
        }

        // now, actually index the source
        if (!CSourceEmitter::createSourcesAtPath(*importedContext, path)) {
            std::cout << "Unable to emit C sources for import" << std::endl;
        }

        // make things in the current context visible
        _importedContexts.push_back(importedContext);

        // need to create function variables for non-C functions
        for (std::pair<std::string, NewDataType> fnPair: importedContext->_functions) {
            if (fnPair.second.kind() == NewDataType::Kind::CFunction) {
                continue;
            }

            this->scope()->defineVariableTypeForName(fnPair.second, fnPair.first);
        }

        CSourceIndexer indexer;

        if (!indexer.indexFileAtPath(path + ".h", this)) {
            std::cout << "Unable to index import header" << std::endl;
            return false;
        }

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

    void ParseContext::setVisibility(TranslationUnit::Visibility visibility) {
        _visibility = visibility;
    }

    TranslationUnit::Visibility ParseContext::visibility() const {
        return _visibility;
    }

    NewDataType ParseContext::dataTypeForName(const std::string& name) const {
        auto it = _dataTypeMap.find(name);

        if (it != _dataTypeMap.cend()) {
            return it->second;
        }

        // search imported context
        for (ParseContext* subcontext : _importedContexts) {
            NewDataType type = subcontext->dataTypeForName(name);

            if (type.defined()) {
                return type;
            }
        }

        return NewDataType();
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

    NewDataType::Kind ParseContext::typeKindWithName(const std::string& name) const {
        return this->dataTypeForName(name).kind();
    }

    bool ParseContext::definesTypeWithName(const std::string& name) const {
        return this->typeKindWithName(name) != NewDataType::Kind::Undefined;
    }

    bool ParseContext::defineTypeForName(NewDataType type, const std::string& name) {
        if (this->definesTypeWithName(name)) {
            return false;
        }

        _dataTypeMap[name] = type;

        return true;
    }

    bool ParseContext::redefineTypeForName(NewDataType type, const std::string& name) {
        if (!this->definesTypeWithName(name)) {
            return false;
        }

        _dataTypeMap[name] = type;

        return true;
    }

    NewDataType ParseContext::functionForName(const std::string& name) const {
        auto it = _functions.find(name);

        if (it != _functions.cend()) {
            return it->second;
        }

        for (ParseContext* subcontext : _importedContexts) {
            NewDataType fnType = subcontext->functionForName(name);

            if (fnType.defined()) {
                return fnType;
            }
        }

        return NewDataType();
    }

    bool ParseContext::defineFunctionForName(const NewDataType& type, const std::string& name) {
        assert((type.kind() == NewDataType::Kind::Function) || (type.kind() == NewDataType::Kind::CFunction));

        if (this->functionForName(name).kind() != NewDataType::Kind::Undefined) {
            return false;
        }

        _functions[name] = type;

        // now, define this function as a "variable" in the current scope
        return this->scope()->defineVariableTypeForName(type, name);
    }

    NewScope* ParseContext::scope() const {
        return _currentScope;
    }

    void ParseContext::pushScope() {
        NewScope* scope = new NewScope();

        scope->setParent(_currentScope);

        _currentScope = scope;
    }

    void ParseContext::popScope() {
        NewScope* old = _currentScope;

        assert(old->parent());
        _currentScope = old->parent();

        delete old;
    }

    void ParseContext::postProcessAST() {
        if (!_rootNode) {
            return;
        }
    }
}
