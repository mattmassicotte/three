#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "closure.h"

#include <three/c11/stdalign.h>

#define THREE_CHECK_SIZEOF(typea, typeb) (sizeof(typea) == sizeof(typeb))
#define THREE_CHECK_ALIGNOF(typea, typeb) (alignof(typea) == alignof(typeb))
#define THREE_CHECK_SIZEALIGN(typea, typeb) (THREE_CHECK_SIZEOF(typea, typeb) && THREE_CHECK_ALIGNOF(typea, typeb))
