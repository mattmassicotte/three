#include "LinkageNode.h"

#include <assert.h>

namespace Three {
    std::string LinkageNode::name() const {
        return "Linkage";
    }

    std::string LinkageNode::libraryName() const {
        return _libraryName;
    }
}
