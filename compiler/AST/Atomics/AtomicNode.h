#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class AtomicNode : public ASTNode {
    public:
        typedef enum {
            Undefined,
            None,
            Consume,
            Acquire,
            Release,
            AcquireRelease,
            SequentiallyConsistent
        } Ordering;
    public:
        static ASTNode* parse(Parser& parser);
        static void parseOrdering(Parser& parser, AtomicNode* node);

    public:
        Ordering ordering() const;

        std::string c11MemoryOrderString() const;

    private:
        Ordering _ordering;
    };
}
