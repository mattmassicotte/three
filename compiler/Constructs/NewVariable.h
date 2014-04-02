#pragma once

#include "compiler/constructs/NewDataType.h"

#include <string>

namespace Three {
    class NewVariable {
    public:
        NewVariable();

    private:
        NewVariable(const NewVariable& other);
        NewVariable(NewVariable&& other);
        NewVariable& operator=(NewVariable other);
        NewVariable& operator=(NewVariable&& other);

    public:
        std::string name;
        NewDataType type;
    };
}
