#pragma once

#include "compiler/constructs/QualifiedName.h"

#include <string>

namespace Three {
    template<typename T>
    class SymbolContainer {
    public:
        SymbolContainer(T placeholder) {
            _missingPlaceholder = placeholder;
        };

        virtual ~SymbolContainer() {
        };

        T symbolForName(const QualifiedName& name, const QualifiedName& containingNamespace = QualifiedName()) const {
            QualifiedName fullName = name;

            fullName.prependName(containingNamespace);

            return this->symbolForExactName(fullName);
        }

        bool defineSymbolForName(T symbol, const QualifiedName& name) {
            auto it = _symbols.find(name.to_s());

            if (it != _symbols.cend()) {
                return false;
            }

            _symbols[name.to_s()] = symbol;

            return true;
        }

        T symbolForExactName(const QualifiedName& name) const {
            auto it = _symbols.find(name.to_s());

            if (it != _symbols.cend()) {
                return it->second;
            }

            return _missingPlaceholder;
        }

    private:
        T _missingPlaceholder;
        std::map<std::string, T> _symbols;
    };
}
