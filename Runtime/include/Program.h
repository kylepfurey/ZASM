// .h
// ZASM Program Class
// by Kyle Furey

#ifndef ZASM_PROGRAM_H
#define ZASM_PROGRAM_H

#include <Coroutine.h>
#include <FileStream.h>
#include <Library.h>

/** All data related to the runtime of a single ZASM program. */
typedef struct {
    /**
     * A vector of pointers to coroutines (each coroutine slot).
     * A NULL pointer indicates an inactive slot.
     */
    ZVector coroutines;

    /** The index of the current coroutine slot. */
    ZUInt current;

    /** The index of the next inactive coroutine slot. */
    ZUInt next;

    /** A vector of pointers to file streams in global order. */
    ZVector files;

    /** The C standard library. */
    ZLibrary libc;
} ZProgram;

#ifdef __cplusplus
extern "C" {
#endif

/** Returns the time in milliseconds. */
ZASM_API ZULong ZTime(ZUInt offsetMs);

/** Initializes a new ZASM program. */
ZASM_API ZBool ZProgram_new(ZProgram *self, ZString path, ZUInt argc, const ZString argv[]);

/** Starts a new coroutine in a ZASM program. */
ZASM_API ZBool ZProgram_startCoroutine(
    ZProgram *self,
    ZUInt handleStart,
    ZUInt argSize,
    ZUInt globalOffset
);

/** Stops a coroutine by its index in a ZASM program. Only "fails" on stopping main. */
ZASM_API ZBool ZProgram_stopCoroutine(ZProgram *self, ZUInt index, ZBool dispatch);

/** Loads a .zlib file at the given path into the ZASM program. */
ZASM_API ZBool ZProgram_loadLibrary(ZProgram *self, ZString path);

/** Binds a foreign function to the ZASM program. */
ZASM_API ZBool ZProgram_bind(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZUInt library
);

/** Calls a foreign function from the ZASM program's libraries. */
ZASM_API ZBool ZProgram_call(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZUInt library,
    ZUInt ffi
);

/** Executes the next coroutine in a ZASM program. Returns whether the program can continue. */
ZASM_API ZBool ZProgram_step(ZProgram *self);

/** Runs a ZASM program until completion. Returns the result of main. */
ZASM_API ZInt ZProgram_execute(ZProgram *self);

/** Cleans up all memory owned by a ZASM program. */
ZASM_API void ZProgram_delete(ZProgram *self);

#ifdef __cplusplus
}
#endif

#endif // ZASM_PROGRAM_H
