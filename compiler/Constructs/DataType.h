#pragma once

#include "Entity.h"
#include "TypeReference.h"
#include "../CodeGen/CSource.h"

#include <functional>
#include <vector>
#include <sstream>

namespace Three {
    class DataType : public Three::Entity {
    public:
        typedef enum {
            Undefined,
            Scalar,
            Structure,
            Union,
            Function,
            Closure
        } Flavor;
    public:
        DataType(const Flavor& type);
        DataType(const Flavor& type, const std::string& name);

        std::string str() const;

        Flavor flavor() const;
        void setFlavor(const Flavor& value);
        std::string fullyQualifiedName() const;
        std::string cSourceName() const;
        std::string cHeader() const;
        bool cSourcePrependStructKeyword() const;
        void setCSourcePrependStructKeyword(bool prepend);

        uint32_t childCount() const;
        void addChild(const TypeReference& value, const std::string& name);
        TypeReference childAtIndex(uint32_t index) const;
        TypeReference childForName(const std::string& name) const;
        void eachChild(std::function<void (const TypeReference&, const std::string&, uint32_t)> func) const;
        
        bool isCallable() const;

        TypeReference returnType() const;
        void          setReturnType(const TypeReference& value);
        void          eachParameterWithLast(std::function<void (const TypeReference&, const std::string&, bool)> func) const;

    private:
        Flavor _type;
        bool _prependStructKeyword;

        TypeReference              _returnType;
        std::vector<TypeReference> _children;
        std::vector<std::string>   _childNames;
    };
}
