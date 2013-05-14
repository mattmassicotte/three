#include "Module.h"

namespace Language {
    Module::Module() {
    }

    Module::~Module() {
    }

    void Module::setCIncludePath(const std::string& value) {
        _cIncludePath = value;
    }

    std::string Module::cIncludePath() const {
        return _cIncludePath;
    }

}
