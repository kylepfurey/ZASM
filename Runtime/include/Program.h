// .h
// Z Program Class
// by Kyle Furey

#ifndef ZLANG_PROGRAM_H
#define ZLANG_PROGRAM_H

#include <Vector.h>

/** All data related to the runtime of a single Z program. */
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
} ZProgram;

/** Initializes a new Z program. */
ZLANG_API ZBool ZProgram_new(ZProgram *self, ZString path, ZUInt argc, const ZString argv[]);

/** Starts a new coroutine in a Z program. */
ZLANG_API ZBool ZProgram_startCoroutine(
    ZProgram *self,
    ZUInt handleStart,
    ZUInt argSize,
    ZUInt globalOffset
);

/** Stops a coroutine by its index in a Z program. Only "fails" on stopping main. */
ZLANG_API ZBool ZProgram_stopCoroutine(ZProgram *self, ZUInt index, ZBool dispatch);

/** Loads a .zlib file at the given path into the Z program. */
ZLANG_API ZBool ZProgram_loadLibrary(ZProgram *self, ZString path);

/** Executes the next coroutine in a Z program. Returns whether the program can continue. */
ZLANG_API ZBool ZProgram_step(ZProgram *self);

/** Runs a Z program until completion. Returns the result of main. */
ZLANG_API ZInt ZProgram_execute(ZProgram *self);

/** Cleans up all memory owned by a Z program. */
ZLANG_API void ZProgram_delete(ZProgram *self);

#endif // ZLANG_PROGRAM_H
