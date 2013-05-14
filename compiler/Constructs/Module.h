#pragma once

#include <string>

namespace Language {
    class Module {
    public:
        Module();
        virtual ~Module();

        void setCIncludePath(const std::string& value);
        std::string cIncludePath() const;

    private:
        std::string _cIncludePath;
    };
}
