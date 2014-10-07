#pragma once

#include "compiler/constructs/NewDataType.h"
#include "compiler/Constructs/TranslationUnit.h"

#include <string>
#include <vector>

namespace Three {
    class ASTNode;
    class RootNode;
    class Message;
    class NewScope;
    class NewVariable;
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

        NewDataType dataTypeForName(const QualifiedName& name) const;
        bool identifierAvailableForDefinition(const std::string& name) const;

        bool definesTypeWithName(const std::string& name) const;
        bool definesTypeWithName(const QualifiedName& name) const;
        NewDataType::Kind typeKindWithName(const std::string& name) const;

        bool defineTypeForName(NewDataType type, const std::string& name);
        bool redefineTypeForName(NewDataType type, const std::string& name);

        NewDataType functionForName(const std::string& name) const;
        bool defineFunctionForName(const NewDataType& type, const std::string& name);

        NewVariable* variableForName(const QualifiedName& name) const;
        bool defineVariable(NewVariable* variable, bool scoped=true);
        bool defineVariableTypeForName(const NewDataType& type, const std::string& name, bool scoped=true);

        NewScope* scope() const;
        void pushScope();
        void popScope();
        NewScope* rootScope() const;

        void postProcessAST();

    private:
        NewVariable* variableForExactName(const std::string& name) const;
        NewDataType dataTypeForExactName(const std::string& name) const;

    public:
        // for testing
        bool skipIncludes;
        bool skipImports;

    private:
        RootNode* _rootNode;
        NewScope* _rootScope;
        NewScope* _currentScope;
        TranslationUnit::Visibility _visibility;

        std::vector<Message*> _messages;
        std::map<std::string, NewDataType> _dataTypeMap;
        std::map<std::string, NewDataType> _functions;
        std::map<std::string, NewVariable*> _variables;

        std::vector<ParseContext*> _importedContexts;
        std::vector<std::string> _importedPaths;
        std::vector<std::string> _importSearchPaths;
    };
}
