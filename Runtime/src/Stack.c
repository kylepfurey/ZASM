// .c
// ZASM Stack Class
// by Kyle Furey

#include <ZASM.h>

/** Initializes a new stack. This does not need to be deleted. */
ZBool ZStack_new(ZStack *self, ZUInt pushed) {
    Zassert(self != NULL, "<self> was NULL!");
    self->top = self->bottom;
    return ZStack_push(self, pushed);
}

/** Pushes bytes to the stack. */
ZBool ZStack_push(ZStack *self, ZUInt size) {
    Zassert(self != NULL, "<self> was NULL!");
    if (self->top + size > self->bottom + ZASM_STACK_SIZE) {
        Zerror("Stack overflow!");
        return false;
    }
    self->top += size;
    return true;
}

/** Pops bytes from the stack. */
ZBool ZStack_pop(ZStack *self, ZUInt size) {
    Zassert(self != NULL, "<self> was NULL!");
    if (self->top - size < self->bottom) {
        Zerror("Stack underflow!");
        return false;
    }
    self->top -= size;
    return true;
}

/** Returns a pointer to the stack data at the given top offset. */
void *ZStack_peekTop(ZStack *self, ZUInt offset, ZUInt size) {
    Zassert(self != NULL, "<self> was NULL!");
    ZUInt max = (ZUInt) (self->top - self->bottom);
    if (offset > max || size > offset) {
        Zerror("Invalid stack range!");
        return NULL;
    }
    return self->top - offset;
}

/** Returns a pointer to the stack data at the given bottom offset. */
void *ZStack_peekBottom(ZStack *self, ZUInt offset, ZUInt size) {
    Zassert(self != NULL, "<self> was NULL!");
    ZUInt max = (ZUInt) (self->top - self->bottom);
    if (offset > max || size > max - offset) {
        Zerror("Invalid stack range!");
        return NULL;
    }
    return self->bottom + offset;
}

/** Returns the current size of the stack. */
ZUInt ZStack_size(const ZStack *self) {
    Zassert(self != NULL, "<self> was NULL!");
    return (ZUInt) (self->top - self->bottom);
}
