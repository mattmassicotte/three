#include "RootNode.h"

namespace Language {
    std::string RootNode::name() const {
        return "Root";
    }

    void RootNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        stream << "// Generated C Code" << std::endl;

        for (int i = 0; i < this->childCount(); ++i) {
            this->childAtIndex(i)->renderCCode(stream, 0);
        }
    }
}
