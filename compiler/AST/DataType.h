#pragma once

#include "../Parser.h"

#include <sstream>
#include <string>

namespace Language {
    class DataType {
    public:
        static DataType parse(Parser& parser);
        static DataType voidType();

    public:
        DataType();

        std::string str() const;

        void setIndirectionDepth(uint32_t depth);
        uint32_t indirectionDepth() const; 
        void setTypeName(const std::string& string);
        std::string typeName() const;

        virtual void renderCCode(std::stringstream& stream, uint32_t indentation);

    private:
        uint32_t    _indirectionDepth;
        std::string _typeName;
    };
}
