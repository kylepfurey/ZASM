// .c
// ZASM File Stream Class
// by Kyle Furey

#include <ZASM.h>

/** Initializes a new file stream. */
ZBool ZFileStream_new(ZFileStream *self, ZString path, ZULong globalOffset) {
    Zassert(path != NULL, "<path> was NULL!");
    Zassert(self != NULL, "<self> was NULL!");
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        ZString home = getenv(ZASM_HOME_VAR);
        if (home == NULL) {
            Zerror("File not found in directory!");
            return false;
        }
        ZULong homeLen = strlen(home);
        ZULong pathLen = strlen(path);
        ZChar *buffer = (ZChar *) malloc(homeLen + pathLen + 2);
        if (buffer == NULL) {
            Zerror("Could not allocate buffer for Z_HOME path + file path!");
            return false;
        }
        memcpy(buffer, home, homeLen);
        buffer[homeLen] = ZASM_PATH_SEPARATOR;
        memcpy(buffer + homeLen + 1, path, pathLen);
        buffer[homeLen + pathLen + 1] = '\0';
        file = fopen(buffer, "rb");
        free(buffer);
        if (file == NULL) {
            Zerror("File not found in directory or Z_HOME!");
            return false;
        }
    }
    self->chunkSize = fread(self->chunk, 1, ZASM_CHUNK_SIZE, file);
    if (self->chunkSize == 0 || self->chunk[0] != ZOPCODE_MAGIC) {
        Zerror("File does not start with the Z byte!");
        fclose(file);
        return false;
    }
    if (self->chunkSize == 1 ||
        fseek(file, -1, SEEK_END) != 0 ||
        fgetc(file) != ZOPCODE_MAGIC) {
        Zerror("File does not end with the Z byte!");
        fclose(file);
        return false;
    }
    self->globalOffset = globalOffset;
    self->chunkIndex = 0;
    self->byteIndex = 1;
    if (fseek(file, 0, SEEK_END) != 0) {
        Zerror("Could not set file stream to end!");
        fclose(file);
        return false;
    }
    ZLong size = ftell(file);
    if (size <= 0) {
        Zerror("Empty file!");
        fclose(file);
        return false;
    }
    self->fileSize = (ZULong) size;
    if (fseek(file, ZASM_CHUNK_SIZE, SEEK_SET) != 0) {
        Zerror("Could not set file stream to second chunk!");
        fclose(file);
        return false;
    }
    self->file = file;
    if (!ZVector_new(&self->libraries, ZASM_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize libraries vector!");
        fclose(file);
        self->file = NULL;
        return false;
    }
    if (!ZVector_new(&self->types, ZASM_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize types vector!");
        ZVector_delete(&self->libraries);
        fclose(file);
        self->file = NULL;
        return false;
    }
    return true;
}

/** Outputs the next byte of a file stream, iterating chunks when needed. */
ZBool ZFileStream_nextByte(ZFileStream *self, ZByte *byte, ZCoroutine *coro) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->file != NULL, "<self>'s FILE handle was NULL!");
    Zassert(byte != NULL, "<byte> was NULL!");
    if (self->byteIndex >= self->chunkSize) {
        return false;
    }
    *byte = self->chunk[self->byteIndex++];
    if (self->byteIndex == self->chunkSize) {
        self->byteIndex = 0;
        ++self->chunkIndex;
        self->chunkSize = fread(self->chunk, 1, ZASM_CHUNK_SIZE, self->file);
        if (self->chunkSize == 0) {
            return false;
        }
    }
    if (coro != NULL) {
        coro->globalOffset = ZFileStream_globalOffset(self);
    }
    return true;
}

/** Outputs an array of bytes from a file stream, iterating chunks when needed. */
ZBool ZFileStream_nextArray(ZFileStream *self, ZUInt size, ZByte *array, ZCoroutine *coro) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->file != NULL, "<self>'s FILE handle was NULL!");
    Zassert(array != NULL, "<array> was NULL!");
    if (ZASM_LITTLE_ENDIAN) {
        for (; size > 0; --size) {
            if (!ZFileStream_nextByte(self, array++, coro)) {
                return false;
            }
        }
    } else {
        array += size;
        for (; size > 0; --size) {
            if (!ZFileStream_nextByte(self, --array, coro)) {
                return false;
            }
        }
    }
    return true;
}

/** Jumps to the given byte index in a file stream. */
ZBool ZFileStream_jumpLocal(ZFileStream *self, ZULong localOffset) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->file != NULL, "<self>'s FILE handle was NULL!");
    ZUInt chunk = (localOffset / ZASM_CHUNK_SIZE) * ZASM_CHUNK_SIZE;
    if (self->chunkIndex != chunk) {
        self->chunkIndex = chunk;
        if (fseek(self->file, chunk, SEEK_SET) != 0) {
            Zerror("Invalid local jump!");
            return false;
        }
        self->chunkSize = fread(self->chunk, 1, ZASM_CHUNK_SIZE, self->file);
    }
    self->byteIndex = localOffset % ZASM_CHUNK_SIZE;
    return self->byteIndex < self->chunkSize;
}

/** Jumps and returns the file stream at the given global offset in an array of file streams. */
ZFileStream *ZFileStream_jumpGlobal(
    ZUInt fileCount,
    ZFileStream *files[],
    ZULong globalOffset,
    ZCoroutine *coro
) {
    Zassert(files != NULL, "<files> was NULL!");
    while (fileCount > 0 && !ZFileStream_inRange(*files, globalOffset)) {
        globalOffset -= files[0]->fileSize;
        ++files;
        --fileCount;
    }
    if (fileCount == 0) {
        Zerror("Invalid global jump!");
        return NULL;
    }
    if (!ZFileStream_jumpLocal(files[0], globalOffset)) {
        Zerror("Invalid global jump!");
        return NULL;
    }
    if (coro != NULL) {
        coro->globalOffset = globalOffset;
    }
    return files[0];
}

/** Returns the current byte index of a file stream. */
ZULong ZFileStream_localOffset(const ZFileStream *self) {
    Zassert(self != NULL, "<self> was NULL!");
    return (self->chunkIndex * ZASM_CHUNK_SIZE) + self->byteIndex;
}

/** Returns the current global offset of a file stream. */
ZULong ZFileStream_globalOffset(const ZFileStream *self) {
    Zassert(self != NULL, "<self> was NULL!");
    return ZFileStream_localOffset(self) + self->globalOffset;
}

/** Returns whether a global offset is in range of a file stream. */
ZBool ZFileStream_inRange(const ZFileStream *self, ZULong globalOffset) {
    Zassert(self != NULL, "<self> was NULL!");
    return globalOffset >= self->globalOffset && globalOffset < self->globalOffset + self->fileSize;
}

/** Loads a dynamic library with the given name into the file stream. */
ZBool ZFileStream_loadLibrary(ZFileStream *self, ZString name) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(name != NULL, "<name> was NULL!");
    ZLibrary *lib = (ZLibrary *) malloc(sizeof(ZLibrary));
    if (lib == NULL) {
        Zerror("Could not allocate dynamic library!");
        return false;
    };
    if (!ZLibrary_new(lib, name)) {
        Zerror("Could not initialize dynamic library!");
        free(lib);
        return false;
    }
    if (!ZVector_push(&self->libraries, (ZULong) lib)) {
        Zerror("Could not insert dynamic library!");
        ZLibrary_delete(lib);
        free(lib);
        return false;
    }
    return true;
}

/** Stores a new struct type in the file stream. */
ZBool ZFileStream_putType(ZFileStream *self, ZUInt count, ZUInt elements[]) {
    Zassert(self != NULL, "<self> was NULL!");
    ZType *newType = (ZType *) malloc(sizeof(ZType));
    if (newType == NULL) {
        Zerror("Could not allocate new type!");
        return false;
    }
    ZType **types = (ZType **) malloc(count * sizeof(ZType *));
    if (types == NULL) {
        Zerror("Could not allocate new type's element array!");
        free(newType);
        return false;
    }
    for (ZUInt i = 0; i < count; ++i) {
        ZType *type = ZFileStream_getType(self, elements[i]);
        if (type == NULL) {
            Zerror("Nonexistent type!");
            free(types);
            free(newType);
            return false;
        }
        types[i] = type;
    }
    newType->size = 0;
    newType->alignment = 0;
    newType->type = FFI_TYPE_STRUCT;
    newType->elements = (ffi_type **) types;
    if (!ZVector_push(&self->types, (ZULong) newType)) {
        Zerror("Could not insert new type!");
        free(types);
        free(newType);
        return false;
    }
    return true;
}

/** Returns a pointer to the file stream type with the given index. */
ZType *ZFileStream_getType(const ZFileStream *self, ZUInt index) {
    Zassert(self != NULL, "<self> was NULL!");
    switch (index) {
        case ZASM_TYPE_VOID:
            return &ffi_type_void;
        case ZASM_TYPE_BYTE:
            return &ffi_type_uint8;
        case ZASM_TYPE_SBYTE:
            return &ffi_type_sint8;
        case ZASM_TYPE_USHORT:
            return &ffi_type_uint16;
        case ZASM_TYPE_SHORT:
            return &ffi_type_sint16;
        case ZASM_TYPE_UINT:
            return &ffi_type_uint32;
        case ZASM_TYPE_INT:
            return &ffi_type_sint32;
        case ZASM_TYPE_ULONG:
            return &ffi_type_uint64;
        case ZASM_TYPE_LONG:
            return &ffi_type_sint64;
        case ZASM_TYPE_FLOAT:
            return &ffi_type_float;
        case ZASM_TYPE_DOUBLE:
            return &ffi_type_double;
        case ZASM_TYPE_PTR:
            return &ffi_type_pointer;
        case ZASM_TYPE_DECIMAL:
            return &ffi_type_longdouble;
        default:
            if (index < self->types.count) {
                return (ZType *) ZVector_get(&self->types, index - ZASM_TYPE_STRUCT);
            }
            return NULL;
    }
}

/** Cleans up all memory owned by a file stream. */
void ZFileStream_delete(ZFileStream *self) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->file != NULL, "<self>'s FILE handle was NULL!");
    ZUInt count = self->types.count;
    for (ZUInt i = 0; i < count; ++i) {
        ZType *type = (ZType *) ZVector_get(&self->types, i);
        free(type->elements);
        free(type);
    }
    ZVector_delete(&self->types);
    count = self->libraries.count;
    for (ZUInt i = 0; i < count; ++i) {
        ZLibrary *lib = (ZLibrary *) ZVector_get(&self->libraries, i);
        ZLibrary_delete(lib);
        free(lib);
    }
    ZVector_delete(&self->libraries);
    fclose(self->file);
    self->file = NULL;
}
