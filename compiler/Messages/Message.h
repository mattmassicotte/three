#pragma once

#include <string>

namespace Three {
    class Message {
    public:
        Message();
        virtual ~Message();

    public:
        std::string str() const;

        virtual std::string name() const = 0;
        virtual std::string message() const = 0;
        virtual std::string file() const;
        virtual uint32_t lineNumber() const;
        virtual uint32_t offset() const;

        virtual uint32_t calloutStartOffset() const;
        virtual uint32_t calloutEndOffset() const;

    private:
        std::string _file;
        uint32_t _lineNumber;
        uint32_t _offset;
    };
}
