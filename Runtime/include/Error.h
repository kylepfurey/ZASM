// .h
// Z Error Logging Macros
// by Kyle Furey

#ifndef ZLANG_ERROR_H
#define ZLANG_ERROR_H

#include <Types.h>

/** Returned when the Z runtime encounters an error. */
#define ZLANG_ERROR 'Z'

#ifdef ZLANG_DEBUG

/** Logs a message to the Z runtime when debugging. */
#define Zlog(...)\
do {\
    fprintf(stdout, "Z LOG:\t\t");\
    fprintf(stderr, "%s() - ", __func__);\
    fprintf(stdout, __VA_ARGS__);\
    fprintf(stdout, "\n");\
} while (false)

#else

/** Noop. */
#define Zlog(...) do {} while(false)

#endif

#ifdef ZLANG_DEBUG

/** Logs an error to the Z runtime when debugging. */
#define Zerror(...)\
do {\
    fprintf(stderr, "Z ERROR:\t");\
    fprintf(stderr, "%s() - ", __func__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
} while (false)

#else

/** Noop. */
#define Zerror(...) do {} while(false)

#endif

#ifdef ZLANG_DEBUG

/** Crashes the Z runtime with the given message msg if cond is false. */
#define Zassert(cond, msg)\
if (!(cond))\
fprintf(\
    stderr,\
    "Z ASSERTION FAILED:\tFunc %s() - Line %i - %s\n",\
    __func__,\
    __LINE__,\
    (msg)\
),\
exit(ZLANG_ERROR)

#else

/** Noop. */
#define Zassert(cond, msg) do {} while(false)

#endif

#endif // ZLANG_ERROR_H
