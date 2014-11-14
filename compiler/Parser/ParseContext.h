#pragma once

#include "compiler/constructs/DataType.h"
#include "compiler/Constructs/TranslationUnit.h"

#include <string>
#include <vector>

namespace Three {
    class ASTNode;
    class RootNode;
    class Message;
    class Scope;
    class Variable;
    class QualifiedName;
}

namespace Three {
    class ParseContext {
    public:
        ParseContext();
        virtual ~ParseContext();

    public:
        RootNode* rootNode() const;

        uint32_t messageCount() const;
        Message* messageAtIndex(uint32_t i) const;
        void addMessage(Message* message);
        void clearMessages();
        bool displayMessages();

        bool import(const std::string& name);
        void addImportSearchPath(const std::string& name);
        std::string resolveImportPath(const std::string& name);
        std::vector<std::string> importedPaths() const;
        bool hasImported(const std::string& path) const;

        void setVisibility(TranslationUnit::Visibility visibility);
        TranslationUnit::Visibility visibility() const;

        DataType dataTypeForName(const QualifiedName& name) const;
        bool identifierAvailableForDefinition(const std::string& name) const;

        bool definesTypeWithName(const std::string& name) const;
        bool definesTypeWithName(const QualifiedName& name) const;
        DataType::Kind typeKindWithName(const std::string& name) const;

        bool defineTypeForName(DataType type, const std::string& name);
        bool redefineTypeForName(DataType type, const std::string& name);

        DataType functionForName(const std::string& name) const;
        bool defineFunctionForName(const DataType& type, const std::string& name);

        Variable* variableForName(const QualifiedName& name) const;
        bool defineVariable(Variable* variable, bool scoped=true);
        bool defineVariableTypeForName(const DataType& type, const std::string& name, bool scoped=true);

        Scope* scope() const;
        void pushScope();
        void popScope();
        Scope* rootScope() const;

        void postProcessAST();

    private:
        Variable* variableForExactName(const std::string& name) const;
        DataType dataTypeForExactName(const std::string& name) const;

    public:
        // for testing
        bool skipIncludes;
        bool skipImports;

    private:
        RootNode* _rootNode;
        Scope* _rootScope;
        Scope* _currentScope;
        TranslationUnit::Visibility _visibility;

        std::vector<Message*> _messages;
        std::map<std::string, DataType> _dataTypeMap;
        std::map<std::string, DataType> _functions;
        std::map<std::string, Variable*> _variables;

        std::vector<ParseContext*> _importedContexts;
        std::vector<std::string> _importedPaths;
        std::vector<std::string> _importSearchPaths;
    };
}
