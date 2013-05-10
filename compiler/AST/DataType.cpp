#include "DataType.h"

#include <assert.h>
#include <sstream>

namespace Language {
    DataType DataType::parse(Parser& parser) {
        DataType dataType;
        uint32_t depth = 0;

        // parse '*', if they are there
        while (parser.peek().str().at(0) == '*') {
            parser.next();
            depth++;
        }

        // parse the type identifier
        dataType.setIndirectionDepth(depth);

        dataType.setTypeName(parser.next().str());

        // parser ":<specialization>", if present
        while (parser.peek().str().at(0) == ':') {
            parser.next().str(); // ":"
            parser.next().str(); // "value"
        }

        return dataType;
    }

    std::string DataType::str() const {
        std::stringstream s;

        s << "<indirection: " << this->indirectionDepth() << " " << this->typeName() << ">";

        return s.str();
    }

    void DataType::setIndirectionDepth(uint32_t depth) {
        _indirectionDepth = depth;
    }

    uint32_t DataType::indirectionDepth() const {
        return _indirectionDepth;
    }

    void DataType::setTypeName(const std::string& string) {
        _typeName = string;
    }

    std::string DataType::typeName() const {
        return _typeName;
    }

    void DataType::renderCCode(std::stringstream& stream) {
        if (this->typeName() == "Int") {
            stream << "int";
        } else if (this->typeName() == "Char") {
            stream << "char";
        } else {
            assert(0 && "Unhandled type for C code rendering");
        }

        for (int i = 0; i < this->indirectionDepth(); ++i) {
            stream << "*";
        }
    }
}
