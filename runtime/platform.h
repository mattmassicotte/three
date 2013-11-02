#pragma once

// OSes

// OSX and iOS
#define THREE_PLATFORM_DARWIN (defined(__APPLE__) && defined(__MACH__))
#if THREE_PLATFORM_DARWIN
#include <TargetConditionals.h>
#define THREE_PLATFORM_IOS TARGET_OS_IPHONE
#define THREE_PLATFORM_IOS_SIMULATOR TARGET_IPHONE_SIMULATOR
#define THREE_PLATFORM_IOS_DEVICE TARGET_OS_EMBEDDED
#define THREE_PLATFORM_OSX (TARGET_OS_MAC && !THREE_PLATFORM_IOS)
#endif

// Windows
#define THREE_PLATFORM_WINDOWS (defined(_WIN32) || defined(_WIN64))

// Linux
#define THREE_PLATFORM_LINUX defined(__linux__)
#if THREE_PLATFORM_LINUX
// Define the _GNU_SOURCE macro, so system headers define the expected things
#define _GNU_SOURCE
#endif

// Solaris
#define THREE_PLATFORM_SOLARIS (defined(__sun) && defined(__SVR4))

// Standards/Capabilities

// BSD
#if defined(__unix__) || THREE_PLATFORM_DARWIN
#include <sys/param.h>
#define THREE_PLATFORM_BSD defined(BSD)
#else
#define THREE_PLATFORM_BSD 0
#endif

// UNIX and POSIX
#define THREE_PLATFORM_UNIX !THREE_PLATFORM_WINDOWS && (THREE_PLATFORM_DARWIN || defined(__unix__) || defined(__unix))
#if THREE_PLATFORM_UNIX
#include <unistd.h>
#define THREE_PLATFORM_POSIX defined(_POSIX_VERSION)
#else
#define THREE_PLATFORM_POSIX 0
#endif
