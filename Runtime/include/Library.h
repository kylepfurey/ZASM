// .h
// Z Dynamic Library Class
// by Kyle Furey

#ifndef ZLANG_LIBRARY_H
#define ZLANG_LIBRARY_H

#include <Vector.h>

/** Denotes that an FFI function is not a varadic function. */
#define ZLANG_CALL_NO_VARADIC UINT_MAX

/**
 * Each enumerated primitive type.
 * Custom struct types follow at and after ZLANG_TYPE_STRUCT.
 */
enum {
    ZLANG_TYPE_VOID = 0,
    ZLANG_TYPE_BYTE = 1,
    ZLANG_TYPE_SBYTE = 2,
    ZLANG_TYPE_USHORT = 3,
    ZLANG_TYPE_SHORT = 4,
    ZLANG_TYPE_UINT = 5,
    ZLANG_TYPE_INT = 6,
    ZLANG_TYPE_ULONG = 7,
    ZLANG_TYPE_LONG = 8,
    ZLANG_TYPE_FLOAT = 9,
    ZLANG_TYPE_DOUBLE = 10,
    ZLANG_TYPE_PTR = 11,
    ZLANG_TYPE_DECIMAL = 12,
    ZLANG_TYPE_STRUCT = 13, // >= 13
};

/** A handle to a dynamic library. */
typedef struct {
#ifdef ZLANG_WINDOWS

    /** The handle to the .dll dynamic library. */
    HMODULE handle;

#else

    /** The handle to the .so / .dylib dynamic library. */
    void *handle;

#endif

    /** A vector of pointers to foreign function interfaces. */
    ZVector ffi;
} ZLibrary;

/** Data for a foreign function interface. */
typedef struct {
    /** The interface of this foreign function. */
    ffi_cif cif;

    /** A pointer to the foreign function. */
    ZFunc func;
} ZFFI;

/** Data for a static type. */
typedef struct {
    /** The interface of this static type. */
    ffi_type type;
} ZType;

#pragma pack(push, 1)

/** Metadata for a foreign function interface call. */
typedef struct {
    /** The ABI of this foreign function call. */
    ZUInt abi;

    /** The total number of typed arguments. Argument type indicies follow this structure. */
    ZUInt argCount;

    /** The total number of varadic arguments. ZLANG_CALL_NO_VARADIC ignores this. */
    ZUInt varCount;

    /** The type index of the return type. */
    ZUInt returnType;
} __attribute__((packed)) ZCall;

#pragma pack(pop)

/** Initializes a new dynamic library. */
ZLANG_API ZBool ZLibrary_new(ZLibrary *self, ZString name);

/** Returns a function pointer from a dynamic library via its name. */
ZLANG_API ZFunc ZLibrary_find(ZLibrary *self, ZString func);

/** Cleans up all memory owned by a dynamic library. */
ZLANG_API void ZLibrary_delete(ZLibrary *self);

#endif // ZLANG_LIBRARY_H
