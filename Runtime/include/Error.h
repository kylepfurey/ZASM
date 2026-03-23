// .h
// ZASM Error Logging Macros
// by Kyle Furey

#ifndef ZASM_ERROR_H
#define ZASM_ERROR_H

#include <Types.h>

/** Returned when the ZASM runtime encounters an error. */
#define ZASM_ERROR 'Z'

#ifdef ZASM_DEBUG

/** Logs a message to the ZASM runtime when debugging. */
#define Zlog(...)\
do {\
    fprintf(stdout, "ZASM LOG:\t");\
    fprintf(stdout, "%s() - ", __func__);\
    fprintf(stdout, __VA_ARGS__);\
    fprintf(stdout, "\n");\
} while (false)

#else

/** Noop. */
#define Zlog(...) do {} while(false)

#endif

#ifdef ZASM_DEBUG

/** Logs an error to the ZASM runtime when debugging. */
#define Zerror(...)\
do {\
    fprintf(stderr, "ZASM ERROR:\t");\
    fprintf(stderr, "%s() - ", __func__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
} while (false)

#else

/** Noop. */
#define Zerror(...) do {} while(false)

#endif

#ifdef ZASM_DEBUG

/** Crashes the ZASM runtime with the given message msg if cond is false. */
#define Zassert(cond, msg)\
if (!(cond))\
fprintf(\
    stderr,\
    "ZASM ASSERTION FAILED:\tFunc %s() - Line %i - %s\n",\
    __func__,\
    __LINE__,\
    (msg)\
),\
exit(ZASM_ERROR)

#else

/** Noop. */
#define Zassert(cond, msg) do {} while(false)

#endif

#endif // ZASM_ERROR_H
