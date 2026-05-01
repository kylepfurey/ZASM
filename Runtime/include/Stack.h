// .h
// ZASM Stack Class
// by Kyle Furey

#ifndef ZASM_STACK_H
#define ZASM_STACK_H

#include <Types.h>

/** The size of a thread's stack. */
#define ZASM_STACK_SIZE 4000000

/** A single coroutine's stack memory. */
typedef struct {
    /** A pointer to the base of this stack. */
    ZByte bottom[ZASM_STACK_SIZE];

    /** A pointer to the top of this stack. */
    ZByte *top;
} ZStack;

#ifdef __cplusplus
extern "C" {
#endif

/** Initializes a new stack. This does not need to be deleted. */
ZASM_API ZBool ZStack_new(ZStack *self, ZUInt pushed);

/** Pushes bytes to the stack. */
ZASM_API ZBool ZStack_push(ZStack *self, ZUInt size);

/** Pops bytes from the stack. */
ZASM_API ZBool ZStack_pop(ZStack *self, ZUInt size);

/** Returns a pointer to the stack data at the given top offset. */
ZASM_API void *ZStack_peekTop(ZStack *self, ZUInt offset, ZUInt size);

/** Returns a pointer to the stack data at the given bottom offset. */
ZASM_API void *ZStack_peekBottom(ZStack *self, ZUInt offset, ZUInt size);

/** Returns the current size of the stack. */
ZASM_API ZUInt ZStack_size(const ZStack *self);

#ifdef __cplusplus
}
#endif

#endif // ZASM_STACK_H
