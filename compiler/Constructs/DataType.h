#pragma once

#include "DataStructure.h"
#include "../CodeGen/CSourceContext.h"

#include <sstream>

namespace Language {
    class Parser;
}

namespace Language {
    class DataType {
    public:
        static DataType parse(Parser& parser);

    public:
        DataType();

        std::string str() const;

        void setIndirectionDepth(uint32_t depth);
        uint32_t indirectionDepth() const; 
        void setStructure(DataStructure* value);
        DataStructure* structure() const;

        void codeGenCSource(CSourceContext& context);

    private:
        uint32_t       _indirectionDepth;
        DataStructure* _structure;
    };
}
