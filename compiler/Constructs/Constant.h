#pragma once

#include "compiler/constructs/DataType.h"
#include "compiler/constructs/QualifiedName.h"

#include <string>

namespace Three {
    class Constant {
    public:
        Constant();

    private:
        Constant(const Constant& other);
        Constant(Constant&& other);
        Constant& operator=(Constant other);
        Constant& operator=(Constant&& other);

    public:
        QualifiedName name;
        DataType type;
    };
}
