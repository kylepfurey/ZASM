// .c
// Z Coroutine Class
// by Kyle Furey

#include <ZLang.h>

/** Initializes a main coroutine. */
ZBool ZCoroutine_newMain(ZCoroutine *self, ZUInt argc, const ZString argv[]) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(argv != NULL, "<argv> was NULL!");
    if (!ZStack_new(
        &self->stack,
        sizeof(ZInt) + sizeof(ZUInt) + sizeof(ZULong) + sizeof(ZULong)
    )) {
        Zerror("Could not initialize main coroutine stack!");
        return false;
    }
    ZByte *dest = self->stack.bottom;
    memset(dest, 0, sizeof(ZInt)); // <return>
    dest += sizeof(ZInt);
    memcpy(dest, &argc, sizeof(ZUInt)); // <argc>
    dest += sizeof(ZUInt);
    memcpy(dest, &argv, sizeof(ZULong)); // <argv>
    dest += sizeof(ZULong);
    memset(dest, 0, sizeof(ZULong)); // <ret>
    self->globalOffset = 1;
    self->index = ZLANG_COROUTINE_MAIN;
    self->await = 0;
    self->delayMs = 0;
    self->id = (ZUShort) ZTime(0);
    return true;
}

/** Initializes an async coroutine by calling with the given handle. */
ZBool ZCoroutine_newAsync(
    ZCoroutine *self,
    ZUInt handleStart,
    ZUInt argSize,
    ZCoroutine *parent,
    ZULong globalOffset,
    ZUInt index
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(parent != NULL, "<parent> was NULL!");
    Zassert(index != ZLANG_COROUTINE_MAIN, "<index> was main coroutine!");
    if (!ZStack_new(&self->stack, handleStart - 1 + sizeof(ZULong))) {
        Zerror("Could not initialize child coroutine stack!");
        return false;
    }
    ZHandle *handle = (ZHandle *) ZStack_peekTop(
        &parent->stack,
        handleStart,
        sizeof(ZHandle)
    );
    if (handle == NULL) {
        Zerror("Could not move parent coroutine data to child coroutine!");
        return false;
    }
    if (!ZStack_pop(&parent->stack, argSize)) {
        Zerror("Could not pop args from parent coroutine!");
        return false;
    }
    ZUInt start = ZStack_size(&parent->stack) - handleStart;
    handle->valid = false; // <valid>
    memcpy(self->stack.bottom, &handle->data, handleStart - 1); // <return> + <args...>
    memset(self->stack.bottom + handleStart - 1, 0, sizeof(ZULong)); // <ret>
    handle->index = index; // <index>
    self->globalOffset = globalOffset;
    self->index = index;
    self->await = 0;
    self->delayMs = 0;
    self->id = (ZUShort) ZTime(0);
    handle->id = self->id; // <id>
    if (!ZVector_new(&self->dispatcher, ZLANG_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize child coroutine dispatcher vector!");
        return false;
    }
    ZHandlePointer ptr = (ZHandlePointer){parent->index, start};
    if (!ZVector_push(&self->dispatcher, *(ZULong *) &ptr)) {
        Zerror("Could not insert handle pointer into child coroutine dispatcher!");
        ZVector_delete(&self->dispatcher);
        return false;
    }
    return true;
}

/** Binds a handle to a coroutine. This cannot fail because binding is anonymous. */
ZBool ZCoroutine_bind(
    ZCoroutine *self,
    ZUInt coroCount,
    ZCoroutine *coroutines[],
    ZUInt coroIndex,
    ZUInt handleBase
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coroutines != NULL, "<coroutines> was NULL!");
    Zassert(self->index != ZLANG_COROUTINE_MAIN, "Cannot bind to main coroutine!");
    if (coroIndex >= coroCount ||
        coroutines[coroIndex] == NULL ||
        ZStack_peekBottom(
            &coroutines[coroIndex]->stack,
            handleBase,
            sizeof(ZHandle)) == NULL
    ) {
        return true; // safe
    }
    ZHandlePointer ptr = (ZHandlePointer){coroIndex, handleBase};
    if (!ZVector_push(&self->dispatcher, *(ZULong *) &ptr)) {
        return false; // unsafe
    }
    return true; // safe
}

/** Dispatches the return value of a coroutine. This cannot fail because binding is anonymous. */
void ZCoroutine_dispatch(
    const ZCoroutine *self,
    ZUInt coroCount,
    ZCoroutine *coroutines[]
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coroutines != NULL, "<coroutines> was NULL!");
    Zassert(self->index != ZLANG_COROUTINE_MAIN, "Cannot dispatch main coroutine!");
    ZUInt count = self->dispatcher.count;
    for (ZUInt i = 0; i < count; ++i) {
        ZHandlePointer ptr = *(ZHandlePointer *) ZVector_get(&self->dispatcher, i);
        if (ptr.index >= coroCount || coroutines[ptr.index] == NULL) {
            continue;
        }
        ZHandle *handle = (ZHandle *) ZStack_peekBottom(
            &coroutines[ptr.index]->stack,
            ptr.offset,
            sizeof(ZHandle)
        );
        if (handle != NULL &&
            !handle->valid &&
            handle->index == self->index &&
            handle->id == self->id) {
            memcpy(&handle->data, self->stack.bottom, ZStack_size(&self->stack));
        }
    }
}

/** Cleans up all memory owned by a coroutine. */
void ZCoroutine_delete(ZCoroutine *self) {
    Zassert(self != NULL, "<self> was NULL!");
    if (self->index != ZLANG_COROUTINE_MAIN) {
        ZVector_delete(&self->dispatcher);
    }
}
