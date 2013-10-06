#pragma once

#include "../ASTNode.h"

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
        static ASTNode* parse(Parser& parser, bool statement=false);
        static void parseOrdering(Parser& parser, AtomicNode* node);

    public:
        Ordering ordering() const;

        std::string c11MemoryOrderString() const;

    private:
        Ordering _ordering;
    };
}
