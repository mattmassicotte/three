#pragma once

#include "compiler/constructs/DataType.h"
#include "compiler/constructs/QualifiedName.h"

#include <string>

namespace Three {
    class Variable {
    public:
        Variable();

    private:
        Variable(const Variable& other);
        Variable(Variable&& other);
        Variable& operator=(Variable other);
        Variable& operator=(Variable&& other);

    public:
        QualifiedName name;
        DataType type;
        bool global;
    };
}
