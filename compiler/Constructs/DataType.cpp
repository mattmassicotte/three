#include "DataType.h"
#include "../Parser.h"

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

        dataType.setIndirectionDepth(depth);

        // parse the type identifier
        assert(parser.peek().type() == Token::Type::Identifier);

        std::string identifier = parser.next().str();
        DataStructure* structure = parser.currentScope()->dataStructureForName(identifier);

        dataType.setStructure(structure);

        // parser ":<specialization>", if present
        while (parser.peek().str().at(0) == ':') {
            parser.next(); // ":"
            parser.next(); // "value"
        }

        return dataType;
    }

    DataType::DataType() : _indirectionDepth(0) {
    }

    std::string DataType::str() const {
        std::stringstream s;

        s << "<indirection: " << this->indirectionDepth() << " " << this->structure()->name() << ">";

        return s.str();
    }

    void DataType::setIndirectionDepth(uint32_t depth) {
        _indirectionDepth = depth;
    }

    uint32_t DataType::indirectionDepth() const {
        return _indirectionDepth;
    }

    void DataType::setStructure(DataStructure* value) {
        assert(value);
        _structure = value;
    }

    DataStructure* DataType::structure() const {
        return _structure;
    }

    void DataType::codeGenCSource(CSourceContext& context) {
        assert(this->structure());

        if (this->structure()->name() == "Int") {
            context.print("int");
        } else if (this->structure()->name() == "Char") {
            context.print("char");
        } else if (this->structure()->name() == "Void") {
            context.print("void");
        } else {
            assert(0 && "Unhandled type for C code rendering");
        }

        for (int i = 0; i < this->indirectionDepth(); ++i) {
            context.print("*");
        }
    }
}
