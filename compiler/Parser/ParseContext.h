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
}

namespace Three {
    class ParseContext {
    public:
        ParseContext();
        virtual ~ParseContext();

    private:
        ParseContext(const ParseContext& other);
        ParseContext(ParseContext&& other);
        ParseContext& operator=(ParseContext other);
        ParseContext& operator=(ParseContext&& other);

    public:
        RootNode* rootNode() const;

        uint32_t messageCount() const;
        Message* messageAtIndex(uint32_t i) const;
        void addMessage(Message* message);

        void setVisibility(TranslationUnit::Visibility visibility);
        TranslationUnit::Visibility visibility() const;

        NewDataType dataTypeForName(const std::string& name) const;
        bool identifierAvailableForDefinition(const std::string& name) const;

        bool definesTypeWithName(const std::string& name) const;
        NewDataType::Kind typeKindWithName(const std::string& name) const;

        bool defineTypeForName(NewDataType type, const std::string& name);
        bool redefineTypeForName(NewDataType type, const std::string& name);

        NewDataType functionForName(const std::string& name) const;
        bool defineFunctionForName(const NewDataType& type, const std::string& name);

        NewDataType variableForName(const std::string& name) const;
        bool defineVariableForName(const NewDataType& type, const std::string& name);

        NewScope* scope() const;
        void pushScope();
        void popScope();

        void postProcessAST();

    public:
        bool skipIncludes; // for testing

    private:
        RootNode* _rootNode;
        NewScope* _currentScope;
        TranslationUnit::Visibility _visibility;

        std::vector<Message*> _messages;
        std::map<std::string, NewDataType> _dataTypeMap;
        std::map<std::string, NewDataType> _functions;
    };
}
