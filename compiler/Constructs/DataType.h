#pragma once

#include "TypeReference.h"
#include "../CodeGen/CSource.h"

#include <functional>
#include <vector>
#include <sstream>

namespace Language {
    class DataType {
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
        std::string name() const;
        void setName(const std::string& string);
        std::string cSourceName() const;

        uint32_t childCount() const;
        void addChild(const TypeReference& value);
        TypeReference childAtIndex(uint32_t index) const;
        void eachChild(std::function<void (const TypeReference&, uint32_t)> func) const;
        
        bool isCallable() const;

        TypeReference returnType() const;
        void          setReturnType(const TypeReference& value);
        void          eachParameterWithLast(std::function<void (const TypeReference&, bool)> func);

    private:
        Flavor      _type;
        std::string _name;
        
        TypeReference              _returnType;
        std::vector<TypeReference> _children;
    };
}
