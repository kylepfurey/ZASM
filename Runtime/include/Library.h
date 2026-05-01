// .h
// ZASM Dynamic Library Class
// by Kyle Furey

#ifndef ZASM_LIBRARY_H
#define ZASM_LIBRARY_H

#include <Vector.h>
#include <Stack.h>

/** Denotes that an FFI function is not a varadic function. */
#define ZASM_CALL_NO_VARADIC UINT_MAX

/** Denotes that an FFI function uses the default OS ABI. */
#define ZASM_DEFAULT_ABI UINT_MAX

/** Denotes at runtime that an FFI call is for libc. */
#define ZASM_FFI_LIBC UINT_MAX

/**
 * Each enumerated primitive type.
 * Custom struct types follow at and after ZASM_TYPE_STRUCT.
 */
enum {
    ZASM_TYPE_VOID = 0,
    ZASM_TYPE_BYTE = 1,
    ZASM_TYPE_SBYTE = 2,
    ZASM_TYPE_USHORT = 3,
    ZASM_TYPE_SHORT = 4,
    ZASM_TYPE_UINT = 5,
    ZASM_TYPE_INT = 6,
    ZASM_TYPE_ULONG = 7,
    ZASM_TYPE_LONG = 8,
    ZASM_TYPE_FLOAT = 9,
    ZASM_TYPE_DOUBLE = 10,
    ZASM_TYPE_PTR = 11,
    ZASM_TYPE_DECIMAL = 12,
    ZASM_TYPE_STRUCT = 13, // >= 13
};

/** A handle to a dynamic library. */
typedef struct {
#ifdef ZASM_WINDOWS

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
    /** The interface of this foreign function. cif.arg_types must be freed! */
    ffi_cif cif;

    /** A pointer to the foreign function. */
    ZFunc func;

    /**
     * Cached data for the function signature.
     * [0] = Return Stack Offset
     * [1] = Argument Count
     * [...] = Argument Stack Offset
     * This must be freed!
     */
    ZUInt *signature;
} ZFFI;

/** Data for a static type. */
typedef ffi_type ZType;

#pragma pack(push, 1)

/** Metadata for a foreign function interface call. */
typedef struct {
    /** The ABI of this foreign function call. ZASM_DEFAULT_ABI is for default ABI. */
    ZUInt abi;

    /** The total number of typed arguments. Argument type indicies follow this structure. */
    ZUInt fixedArgs;

    /** The total number of varadic arguments. ZASM_CALL_NO_VARADIC ignores this. */
    ZUInt varArgs;

    /** The type index of the return type. */
    ZUInt returnType;
} __attribute__((packed)) ZCall;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/** Initializes a new dynamic library. "libc" loads the standard library. */
ZASM_API ZBool ZLibrary_new(ZLibrary *self, ZString name);

/** Returns a function pointer from a dynamic library via its name. */
ZASM_API ZFunc ZLibrary_find(ZLibrary *self, ZString name);

/** Binds a new foreign function to the library. */
ZASM_API ZBool ZLibrary_bind(
    ZLibrary *self,
    ZString name,
    ZUInt abi,
    ZUInt fixedArgs,
    ZUInt varArgs,
    ZType *returnType,
    ZType *argTypes[]
);

/** Invokes the foreign function stored in the library with the given index. */
ZASM_API ZBool ZLibrary_call(ZLibrary *self, ZUInt index, ZStack *stack);

/** Cleans up all memory owned by a dynamic library. */
ZASM_API void ZLibrary_delete(ZLibrary *self);

#ifdef __cplusplus
}
#endif

#endif // ZASM_LIBRARY_H
