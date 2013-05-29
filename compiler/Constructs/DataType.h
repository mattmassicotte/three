#pragma once

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

        uint32_t childCount() const;
        void eachChild(std::function<void (DataType*, uint32_t)> func) const;

        bool isFunction() const;

        DataType* returnType() const;
        void      setReturnType(DataType* value);
        void      eachParameterWithLast(std::function<void (DataType*, bool)> func);

    private:
        Flavor      _type;
        std::string _name;
        
        DataType*              _returnType;
        std::vector<DataType*> _children;
    };
}
