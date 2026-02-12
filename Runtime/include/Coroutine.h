// .h
// Z Coroutine Class
// by Kyle Furey

#ifndef ZLANG_COROUTINE_H
#define ZLANG_COROUTINE_H

#include <Stack.h>
#include <Vector.h>

/** The number of milliseconds a coroutine is alloted before context-switching. */
#define ZLANG_COROUTINE_DELAY_MS 100

/** The index of the main coroutine. */
#define ZLANG_COROUTINE_MAIN 0

/** Stack memory and metadata for a single coroutine. */
typedef struct {
    /** This coroutine's stack space. */
    ZStack stack;

    /** The cached global offset of this coroutine in virtual memory. */
    ZULong globalOffset;

    /** This coroutine's index. */
    ZUInt index;

    /** The index of a remote coroutine this coroutine is waiting on. 0 is NULL. */
    ZUInt await;

    /** The delay in milliseconds before resuming this coroutine. */
    ZUInt delayMs;

    /** A random number used to identify this coroutine. */
    ZUShort id;

    /** A vector of pointers to handles used to share this coroutine's return value. */
    ZVector dispatcher;
} ZCoroutine;

#pragma pack(push, 1)

/** A handle used to receive the return value of a remote coroutine. */
typedef struct {
    /** Whether this handle contains the remote coroutine's return value. */
    ZBool valid;

    union {
        /** If <valid> is false, this is the index of the remote coroutine. */
        ZUInt index;

        /* If <valid> is true, this is the first byte of the remote coroutine's return value. */
        ZByte data;
    };

    /** A random number used to identify this coroutine. */
    ZUShort id;

    /** Padding to align this handle to 8 bytes. */
    ZByte padding;
} __attribute__((packed)) ZHandle;

#pragma pack(pop)

/** A pointer to a coroutine handle in a remote coroutine's stack. */
typedef struct {
    /** The index of the remote coroutine containing the coroutine handle. */
    ZUInt index;

    /** The offset from the bottom of the stack containing the coroutine handle. */
    ZUInt offset;
} ZHandlePointer;

/** Initializes a main coroutine. */
ZLANG_API ZBool ZCoroutine_newMain(ZCoroutine *self, ZUInt argc, const ZString argv[]);

/** Initializes an async coroutine by calling with the given handle. */
ZLANG_API ZBool ZCoroutine_newAsync(
    ZCoroutine *self,
    ZUInt handleStart,
    ZUInt argSize,
    ZCoroutine *parent,
    ZULong globalOffset,
    ZUInt index
);

/** Binds a handle to a coroutine. This cannot fail because binding is anonymous. */
ZLANG_API ZBool ZCoroutine_bind(
    ZCoroutine *self,
    ZUInt coroCount,
    ZCoroutine *coroutines[],
    ZUInt coroIndex,
    ZUInt handleBase
);

/** Dispatches the return value of a coroutine. This cannot fail because binding is anonymous. */
ZLANG_API void ZCoroutine_dispatch(
    const ZCoroutine *self,
    ZUInt coroCount,
    ZCoroutine *coroutines[]
);

/** Cleans up all memory owned by a coroutine. */
ZLANG_API void ZCoroutine_delete(ZCoroutine *self);

#endif // ZLANG_COROUTINE_H
