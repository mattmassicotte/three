#pragma once

#include <string>
#include <vector>
#include <map>

namespace Three {
    class TranslationUnit {
    public:
        typedef enum {
            Default,
            None,
            Internal,
            External
        } Visibility;

    public:
        static TranslationUnit* createTranslationUnit();

    public:
        TranslationUnit();
        virtual ~TranslationUnit();
    };
}
