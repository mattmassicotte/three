#pragma once

#include "../Parser.h"

#include <string>

namespace Language {
    class DataType {
    public:
        static DataType parse(Parser& parser);

    public:
        std::string str() const;

        void setIndirectionDepth(uint32_t depth);
        uint32_t indirectionDepth() const; 
        void setTypeName(const std::string& string);
        std::string typeName() const;
    private:
        uint32_t    _indirectionDepth;
        std::string _typeName;
    };
}
