#include "Message.h"
#include <sstream>

namespace Three {
    Message::Message() : _lineNumber(0), _offset(0) {
    }

    Message::~Message() {
    }

    std::string Message::str() const {
        std::stringstream s;

        s << this->file() << ":" << this->lineNumber() << " -- ";
        s << this->message();
 
        return s.str();
    }

    std::string Message::file() const {
        return "file";
    }

    uint32_t Message::lineNumber() const {
        return _lineNumber;
    }

    uint32_t Message::offset() const {
        return _offset;
    }

    uint32_t Message::calloutStartOffset() const {
        return 0;
    }

    uint32_t Message::calloutEndOffset() const {
        return 0;
    }
}
