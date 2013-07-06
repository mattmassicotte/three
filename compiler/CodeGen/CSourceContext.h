#pragma once

#include "CSource.h"

#include <string>
#include <sstream>
#include <set>

namespace Language {
    class CSourceContext {
    public:
        typedef enum {
            Declarations,
            Body
        } Section;
    public:
        CSourceContext();
        virtual ~CSourceContext();

        void addHeader(const std::string& header);

        CSource* declarations() const;
        CSource* body() const;
        CSource* current() const;
        void setCurrent(Section section);

        CSourceContext& operator <<(const std::string& string);

        std::string renderToString();
    private:
        std::set<std::string> _headers;

        CSource* _declarationSource;
        CSource* _bodySource;
        CSource* _currentSource;
    };
}
