// .h
// ZASM Execution Function
// by Kyle Furey

#ifndef ZASM_EXECUTE_H
#define ZASM_EXECUTE_H

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Executes a .zac file at the given path, returning 0 or an error code from the program. */
ZASM_API ZInt Zexecute(ZString path, ZUInt argc, const ZString argv[]);

#ifdef __cplusplus
}
#endif

#endif // ZASM_EXECUTE_H
