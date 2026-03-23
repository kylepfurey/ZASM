// .h
// ZASM Vector Class
// by Kyle Furey

#ifndef ZASM_VECTOR_H
#define ZASM_VECTOR_H

#include <Types.h>

/** The default capacity for vectors. */
#define ZASM_DEFAULT_CAPACITY 8

/** A minimal dynamic array. */
typedef struct {
    /** The number of elements in the array. */
    ZUInt count;

    /** The maximum number of elements in the array before expanding. */
    ZUInt capacity;

    /** A pointer to the underlying array of elements. */
    ZULong *array;
} ZVector;

/** Initializes a new vector with the given capacity. */
ZASM_API ZBool ZVector_new(ZVector *self, ZUInt capacity);

/** Pushes data to the back of the vector, expanding if needed. */
ZASM_API ZBool ZVector_push(ZVector *self, ZULong data);

/** Returns the vector's data at the given index. */
ZASM_API ZULong ZVector_get(const ZVector *self, ZUInt index);

/** Updates the vector's data at the given index. */
ZASM_API void ZVector_set(ZVector *self, ZUInt index, ZULong data);

/** Cleans up all memory owned by a vector. */
ZASM_API void ZVector_delete(ZVector *self);

#endif // ZASM_VECTOR_H
