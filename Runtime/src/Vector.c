// .c
// ZASM Vector Class
// by Kyle Furey

#include <ZASM.h>

/** Initializes a new vector with the given capacity. */
ZBool ZVector_new(ZVector *self, ZUInt capacity) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(capacity > 0, "<capacity> cannot be zero!");
    ZULong *array = (ZULong *) malloc(sizeof(ZULong) * capacity);
    if (array == NULL) {
        Zerror("Could not allocate vector!");
        return false;
    }
    self->array = array;
    self->count = 0;
    self->capacity = capacity;
    return true;
}

/** Pushes data to the back of the vector, expanding if needed. */
ZBool ZVector_push(ZVector *self, ZULong data) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->array != NULL, "<self>'s array was NULL!");
    Zassert(self->count <= self->capacity, "<self>'s count was invalid!");
    if (self->count == self->capacity) {
        if (self->capacity > USHRT_MAX / 2) {
            Zerror("Could not reallocate vector!");
            return false;
        }
        ZUInt capacity = self->capacity * 2;
        ZULong *array = (ZULong *) realloc(self->array, sizeof(ZULong) * capacity);
        if (array == NULL) {
            Zerror("Could not reallocate vector!");
            return false;
        }
        self->array = array;
        self->capacity = capacity;
    }
    self->array[self->count++] = data;
    return true;
}

/** Returns the vector's data at the given index. */
ZULong ZVector_get(const ZVector *self, ZUInt index) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->array != NULL, "<self>'s array was NULL!");
    Zassert(self->count <= self->capacity, "<self>'s count was invalid!");
    Zassert(index < self->count, "<index> out of bounds!");
    return self->array[index];
}

/** Updates the vector's data at the given index. */
void ZVector_set(ZVector *self, ZUInt index, ZULong data) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->array != NULL, "<self>'s array was NULL!");
    Zassert(self->count <= self->capacity, "<self>'s count was invalid!");
    Zassert(index < self->count, "<index> out of bounds!");
    self->array[index] = data;
}

/** Cleans up all memory owned by a vector. */
void ZVector_delete(ZVector *self) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->array != NULL, "<self>'s array was NULL!");
    Zassert(self->count <= self->capacity, "<self>'s count was invalid!");
    free(self->array);
    self->array = NULL;
}
