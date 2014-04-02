#include "TranslationUnit.h"

#include <assert.h>
#include <iostream>

namespace Three {
    TranslationUnit* TranslationUnit::createTranslationUnit() {
        TranslationUnit* translationUnit = new TranslationUnit();

        return translationUnit;
    }

    TranslationUnit::TranslationUnit() {
    }

    TranslationUnit::~TranslationUnit() {
    }
}
