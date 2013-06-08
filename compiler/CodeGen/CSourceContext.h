#pragma once

#include "CSource.h"

#include <string>
#include <sstream>
#include <vector>

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

        CSource* headers() const;
        CSource* declarations() const;
        CSource* body() const;
        CSource* current() const;
        void setCurrent(Section section);

        CSourceContext& operator <<(const std::string& string);

        std::string renderToString();
    private:
        CSource* _headerSource;
        CSource* _declarationSource;
        CSource* _bodySource;
        CSource* _currentSource;
    };
}
