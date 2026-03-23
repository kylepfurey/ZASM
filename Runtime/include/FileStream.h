// .h
// ZASM File Stream Class
// by Kyle Furey

#ifndef ZASM_FILESTREAM_H
#define ZASM_FILESTREAM_H

#include <Coroutine.h>
#include <Library.h>

/** The size of a single file chunk loaded into memory. */
#define ZASM_CHUNK_SIZE 1024

/** Evaluates whether the runtime is using little-endian binary. */
#define ZASM_LITTLE_ENDIAN (*(ZByte *) &(ZByte){1} == 1)

/** A handle to a file used to iterate its memory in chunks. */
typedef struct {
    /** The file chunk buffer. */
    ZByte chunk[ZASM_CHUNK_SIZE];

    /** The cached global offset of this file in virtual memory. */
    ZULong globalOffset;

    /** The index of the current file chunk. */
    ZUInt chunkIndex;

    /** The size of the current file chunk. */
    ZUShort chunkSize;

    /** The index of the current byte in the current file chunk. */
    ZUShort byteIndex;

    /** The size of this file's binary. */
    ZULong fileSize;

    /** The file's handle used to iterate file chunks. */
    FILE *file;

    /** Libraries owned by this file. */
    ZVector libraries;

    /** Static types owned by this file. */
    ZVector types;
} ZFileStream;

/** Initializes a new file stream. */
ZASM_API ZBool ZFileStream_new(ZFileStream *self, ZString path, ZULong globalOffset);

/** Outputs the next byte of a file stream, iterating chunks when needed. */
ZASM_API ZBool ZFileStream_nextByte(ZFileStream *self, ZByte *byte, ZCoroutine *coro);

/** Outputs an array of bytes from a file stream, iterating chunks when needed. */
ZASM_API ZBool ZFileStream_nextArray(ZFileStream *self, ZUInt size, ZByte *array, ZCoroutine *coro);

/** Jumps to the given local offset in a file stream. */
ZASM_API ZBool ZFileStream_jumpLocal(ZFileStream *self, ZULong localOffset);

/** Jumps and returns the file stream at the given global offset in an array of file streams. */
ZASM_API ZFileStream *ZFileStream_jumpGlobal(
    ZUInt fileCount,
    ZFileStream *files[],
    ZULong globalOffset,
    ZCoroutine *coro
);

/** Returns the current local offset of a file stream. */
ZASM_API ZULong ZFileStream_localOffset(const ZFileStream *self);

/** Returns the current global offset of a file stream. */
ZASM_API ZULong ZFileStream_globalOffset(const ZFileStream *self);

/** Returns whether a global offset is in range of a file stream. */
ZASM_API ZBool ZFileStream_inRange(const ZFileStream *self, ZULong globalOffset);

/** Loads a dynamic library with the given name into the file stream. */
ZASM_API ZBool ZFileStream_loadLibrary(ZFileStream *self, ZString name);

/** Stores a new struct type in the file stream. */
ZASM_API ZBool ZFileStream_putType(ZFileStream *self, ZUInt count, ZUInt elements[]);

/** Returns a pointer to the file stream type with the given index. */
ZASM_API ZType *ZFileStream_getType(const ZFileStream *self, ZUInt index);

/** Cleans up all memory owned by a file stream. */
ZASM_API void ZFileStream_delete(ZFileStream *self);

#endif // ZASM_FILESTREAM_H
