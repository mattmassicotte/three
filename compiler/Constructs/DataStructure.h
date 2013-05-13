#pragma once

#include <string>

namespace Language {
    class DataStructure {
    public:
        DataStructure(const std::string& name);

        void setName(const std::string& string);
        std::string name() const;

    private:
        std::string _name;
    };
}
