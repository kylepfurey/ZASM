// .h
// ZASM External Library Includes
// by Kyle Furey

#ifndef ZASM_EXTERNAL_H
#define ZASM_EXTERNAL_H

// MACROS
#include <Export.h>

// C STANDARD LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef ZASM_SIGINT

#include <signal.h>

#endif

// EXTERNAL LIBRARIES
#ifdef ZASM_WINDOWS

#include <windows.h>

#endif

#ifdef ZASM_POSIX

#include <dlfcn.h>

#endif

#include <ffi.h>

#endif // ZASM_EXTERNAL_H
