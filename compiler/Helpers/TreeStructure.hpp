#pragma once

#include <functional>
#include <vector>

namespace Language {
    template<class T>
    class TreeStructure {
    public:
        void addChild(T node) {
            _children.push_back(node);
        }

        T childAtIndex(uint32_t i) const {
            return _children.at(i);
        }

        uint32_t childCount() const {
            return _children.size();
        }

        void eachChild(std::function<void (T, uint32_t)> func) const {
            uint32_t index = 0;
            for (T child : _children) {
                func(child, index);
                ++index;
            }
        }

        // Returns true for the last element only.  Surprisingly useful.
        void eachChildWithLast(std::function<void (T, bool)> func) const {
            uint32_t lastIndex = this->childCount() - 1;

            this->eachChild([=] (T child, uint32_t index) {
                func(child, index == lastIndex);
            });
        }

    private:
        std::vector<T> _children;
    };
}


