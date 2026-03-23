// .h
// ZASM Execution Function
// by Kyle Furey

#ifndef ZASM_EXECUTE_H
#define ZASM_EXECUTE_H

#include <Types.h>

/** Executes a .zac file at the given path, returning 0 or an error code from the program. */
ZASM_API ZInt Zexecute(ZString path, ZUInt argc, const ZString argv[]);

#endif // ZASM_EXECUTE_H
