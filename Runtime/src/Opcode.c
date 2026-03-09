// .c
// Z Opcode Functions
// by Kyle Furey

#include <ZLang.h>

/** The table of Z operation codes to functions. */
ZBool (*const ZOpcode_TABLE[256])(ZProgram *, ZCoroutine *, ZFileStream *) = {
    [ZOPCODE_exit] = ZOpcode_exit,
    [ZOPCODE_load] = ZOpcode_load,
    [ZOPCODE_lib] = ZOpcode_lib,
    [ZOPCODE_read] = ZOpcode_read,
    [ZOPCODE_push] = ZOpcode_push,
    [ZOPCODE_pop] = ZOpcode_pop,
    [ZOPCODE_move] = ZOpcode_move,
    [ZOPCODE_set] = ZOpcode_set,
    [ZOPCODE_get] = ZOpcode_get,
    [ZOPCODE_put] = ZOpcode_put,
    [ZOPCODE_ptr] = ZOpcode_ptr,
    [ZOPCODE_jump] = ZOpcode_jump,
    [ZOPCODE_hop] = ZOpcode_hop,
    [ZOPCODE_call] = ZOpcode_call,
    [ZOPCODE_ret] = ZOpcode_ret,
    [ZOPCODE_addr] = ZOpcode_addr,
    [ZOPCODE_eql] = ZOpcode_eql,
    [ZOPCODE_grtr] = ZOpcode_grtr,
    [ZOPCODE_flip] = ZOpcode_flip,
    [ZOPCODE_if] = ZOpcode_if,
    [ZOPCODE_coro] = ZOpcode_coro,
    [ZOPCODE_bind] = ZOpcode_bind,
    [ZOPCODE_yld] = ZOpcode_yld,
    [ZOPCODE_wait] = ZOpcode_wait,
    [ZOPCODE_cncl] = ZOpcode_cncl,
    [ZOPCODE_kill] = ZOpcode_kill,
    [ZOPCODE_inc] = ZOpcode_inc,
    [ZOPCODE_dec] = ZOpcode_dec,
    [ZOPCODE_add] = ZOpcode_add,
    [ZOPCODE_sub] = ZOpcode_sub,
    [ZOPCODE_mult] = ZOpcode_mult,
    [ZOPCODE_div] = ZOpcode_div,
    [ZOPCODE_mod] = ZOpcode_mod,
    [ZOPCODE_and] = ZOpcode_and,
    [ZOPCODE_or] = ZOpcode_or,
    [ZOPCODE_not] = ZOpcode_not,
    [ZOPCODE_xor] = ZOpcode_xor,
    [ZOPCODE_lshf] = ZOpcode_lshf,
    [ZOPCODE_rshf] = ZOpcode_rshf,
    [ZOPCODE_hash] = ZOpcode_hash,
    [ZOPCODE_int] = ZOpcode_int,
    [ZOPCODE_uint] = ZOpcode_uint,
    [ZOPCODE_flt] = ZOpcode_flt,
    [ZOPCODE_ffi] = ZOpcode_ffi,
};

/** Executes a Z program's next operation code. */
ZBool ZOpcode_nextCode(ZProgram *self) {
    Zassert(self != NULL, "<self> was NULL!");
    ZCoroutine *coro = (ZCoroutine *) ZVector_get(&self->coroutines, self->current);
    Zassert(coro != NULL, "Corrupted coroutine index!");
    ZFileStream **files = (ZFileStream **) self->files.array;
    ZFileStream *file = ZFileStream_jumpGlobal(self->files.count, files, coro->globalOffset, NULL);
    Zassert(file != NULL, "Corrupted coroutine global offset!");
    ZByte code;
    if (!ZFileStream_nextByte(file, &code, coro)) {
        Zerror("Could not read next opcode!");
        return false;
    }
    if (ZOpcode_TABLE[code] == NULL) {
        Zerror("Unknown opcode!");
        return false;
    }
    return ZOpcode_TABLE[code](self, coro, file);
}

/** Processes the read argument of a Z operation code. */
ZBool ZOpcode_getReadArg(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZByte flags,
    ZUInt *read
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    Zassert(read != NULL, "<read> was NULL!");
    if (!ZFileStream_nextArray(file, sizeof(ZUInt), (ZByte *) read, coro)) {
        Zerror("Could not read <read> argument literal!");
        return false;
    }
    if ((flags & ZOPCODE_FLAGS_SIZE_LITERAL) == 0) {
        ZUInt *arg = (ZUInt *) ZStack_peekTop(&coro->stack, *read, 0);
        if (arg == NULL) {
            Zerror("Could not find <read> argument on stack!");
            return false;
        }
        *read = *arg;
    }
    return true;
}

/** Processes the write argument of a Z operation code. */
ZBool ZOpcode_getWriteArg(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZByte flags,
    ZUInt *write
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    Zassert(write != NULL, "<write> was NULL!");
    if (!ZFileStream_nextArray(file, sizeof(ZUInt), (ZByte *) write, coro)) {
        Zerror("Could not read <write> argument literal!");
        return false;
    }
    if ((flags & ZOPCODE_FLAGS_SIZE_LITERAL) == 0) {
        ZUInt *arg = (ZUInt *) ZStack_peekTop(&coro->stack, *write, 0);
        if (arg == NULL) {
            Zerror("Could not find <write> argument on stack!");
            return false;
        }
        *write = *arg;
    }
    return true;
}

/** Processes the size argument of a Z operation code. */
ZBool ZOpcode_getSizeArg(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZByte flags,
    ZULong *size
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    Zassert(size != NULL, "<size> was NULL!");
    if (!ZFileStream_nextArray(file, sizeof(ZULong), (ZByte *) size, coro)) {
        Zerror("Could not read <size> argument literal!");
        return false;
    }
    if ((flags & ZOPCODE_FLAGS_SIZE_LITERAL) == 0) {
        ZULong *arg = (ZULong *) ZStack_peekTop(&coro->stack, *size, sizeof(ZULong));
        if (arg == NULL) {
            Zerror("Could not find <size> argument on stack!");
            return false;
        }
        *size = *arg;
    }
    return true;
}

/** exit (void) */
ZBool ZOpcode_exit(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    return false;
}

/** load (size) */
ZBool ZOpcode_load(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    ZChar *path = (ZChar *) malloc(size * sizeof(ZChar));
    if (path == NULL) {
        Zerror("Could not allocate string buffer!");
        return false;
    }
    if (!ZFileStream_nextArray(file, size, (ZByte *) &path, coro)) {
        Zerror("Could not read string literal from file!");
        free(path);
        return false;
    }
    if (!ZProgram_loadLibrary(self, path)) {
        Zerror("Could not load zlib file from string literal!");
        free(path);
        return true;
    }
    free(path);
    if (!ZStack_push(&coro->stack, sizeof(ZULong))) {
        Zerror("Could not push return address!");
        return false;
    }
    void *ret = ZStack_peekTop(&coro->stack, sizeof(ZULong), sizeof(ZULong));
    Zassert(ret != NULL, "Corrupted coroutine stack!");
    memcpy(ret, &coro->globalOffset, sizeof(ZULong));
    ZFileStream *jumpedFile = ZFileStream_jumpGlobal(
        self->files.count,
        (ZFileStream **) self->files.array,
        ((ZFileStream *) ZVector_get(&self->files, self->files.count - 1))->globalOffset + 1,
        coro
    );
    if (jumpedFile == NULL) {
        Zerror("Could not jump to zlib library!");
        return false;
    }
    return true;
}

/** lib (size) */
ZBool ZOpcode_lib(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    ZChar *name = (ZChar *) malloc(size * sizeof(ZChar));
    if (name == NULL) {
        Zerror("Could not allocate string buffer!");
        return false;
    }
    if (!ZFileStream_nextArray(file, size, (ZByte *) &name, coro)) {
        Zerror("Could not read string literal from file!");
        free(name);
        return false;
    }
    if (!ZFileStream_loadLibrary(file, name)) {
        Zerror("Could not load dynamic library from string literal!");
        free(name);
        return true;
    }
    free(name);
    return true;
}

/** read (write, read, size) */
ZBool ZOpcode_read(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZUInt write;
    if (!ZOpcode_getWriteArg(self, coro, file, flags, &write)) {
        Zerror("Could not get <write> argument!");
        return false;
    }
    ZUInt read;
    if (!ZOpcode_getReadArg(self, coro, file, flags, &read)) {
        Zerror("Could not get <read> argument!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    void *writeStack = ZStack_peekTop(&coro->stack, write, 0);
    if (writeStack == NULL) {
        Zerror("Could not find <write> argument on stack!");
        return false;
    }
    ZULong globalOffset = coro->globalOffset;
    ZFileStream *jumpedFile = ZFileStream_jumpGlobal(
        self->files.count,
        (ZFileStream **) self->files.array,
        file->globalOffset + read,
        coro);
    if (jumpedFile == NULL) {
        Zerror("Invalid local jump!");
        return false;
    }
    if (!ZFileStream_nextArray(jumpedFile, size, writeStack, coro)) {
        Zerror("Could not read from file!");
        return false;
    }
    Zassert(ZFileStream_jumpGlobal(
                self->files.count,
                (ZFileStream **) self->files.array,
                globalOffset,
                coro),
            "Corrupted coroutine global offset!");
    return true;
}

/** push (size) */
ZBool ZOpcode_push(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    if (!ZStack_push(&coro->stack, size)) {
        Zerror("Could not push to stack!");
        return false;
    }
    return true;
}

/** pop (size) */
ZBool ZOpcode_pop(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    if (!ZStack_pop(&coro->stack, size)) {
        Zerror("Could not pop from stack!");
        return false;
    }
    return true;
}

/** move (write, read, size) */
ZBool ZOpcode_move(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZUInt write;
    if (!ZOpcode_getWriteArg(self, coro, file, flags, &write)) {
        Zerror("Could not get <write> argument!");
        return false;
    }
    ZUInt read;
    if (!ZOpcode_getReadArg(self, coro, file, flags, &read)) {
        Zerror("Could not get <read> argument!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    void *writeStack = ZStack_peekTop(&coro->stack, write, 0);
    if (writeStack == NULL) {
        Zerror("Could not find <write> argument on stack!");
        return false;
    }
    void *readStack = ZStack_peekTop(&coro->stack, read, 0);
    if (readStack == NULL) {
        Zerror("Could not find <read> argument on stack!");
        return false;
    }
    memcpy(writeStack, readStack, size);
    return true;
}

/** set (write, read, size) */
ZBool ZOpcode_set(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZUInt write;
    if (!ZOpcode_getWriteArg(self, coro, file, flags, &write)) {
        Zerror("Could not get <write> argument!");
        return false;
    }
    ZUInt read;
    if (!ZOpcode_getReadArg(self, coro, file, flags, &read)) {
        Zerror("Could not get <read> argument!");
        return false;
    }
    ZULong size;
    if (!ZOpcode_getSizeArg(self, coro, file, flags, &size)) {
        Zerror("Could not get <size> argument!");
        return false;
    }
    void *writeStack = ZStack_peekTop(&coro->stack, write, 0);
    if (writeStack == NULL) {
        Zerror("Could not find <write> argument on stack!");
        return false;
    }
    memset(writeStack, (int) read, size);
    return true;
}

/** get (write, read, size) */
ZBool ZOpcode_get(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** put (write, read, size) */
ZBool ZOpcode_put(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** ptr (write, read) */
ZBool ZOpcode_ptr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZByte flags;
    if (!ZFileStream_nextByte(file, &flags, coro)) {
        Zerror("Could not find opcode flags!");
        return false;
    }
    ZUInt write;
    if (!ZOpcode_getWriteArg(self, coro, file, flags, &write)) {
        Zerror("Could not get <write> argument!");
        return false;
    }
    ZUInt read;
    if (!ZOpcode_getReadArg(self, coro, file, flags, &read)) {
        Zerror("Could not get <read> argument!");
        return false;
    }
    void *writeStack = ZStack_peekTop(&coro->stack, write, 0);
    if (writeStack == NULL) {
        Zerror("Could not find <write> argument on stack!");
        return false;
    }
    void *readStack = ZStack_peekTop(&coro->stack, read, 0);
    if (readStack == NULL) {
        Zerror("Could not find <read> argument on stack!");
        return false;
    }
    memcpy(writeStack, &readStack, sizeof(void *));
    return true;
}

/** jump (size) */
ZBool ZOpcode_jump(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** hop (size) */
ZBool ZOpcode_hop(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** call (size) */
ZBool ZOpcode_call(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** ret (size) */
ZBool ZOpcode_ret(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** addr (write, read) */
ZBool ZOpcode_addr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** eql (write, read, size) */
ZBool ZOpcode_eql(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** grtr (write, read, size) */
ZBool ZOpcode_grtr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** flip (write) */
ZBool ZOpcode_flip(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** if (read, size) */
ZBool ZOpcode_if(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** coro (write, read, size) */
ZBool ZOpcode_coro(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** bind (read) */
ZBool ZOpcode_bind(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** yld (read) */
ZBool ZOpcode_yld(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** wait (read) */
ZBool ZOpcode_wait(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** cncl (read) */
ZBool ZOpcode_cncl(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** kill (void) */
ZBool ZOpcode_kill(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** inc (write) */
ZBool ZOpcode_inc(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** dec (write) */
ZBool ZOpcode_dec(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** add (write, read, size) */
ZBool ZOpcode_add(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** sub (write, read, size) */
ZBool ZOpcode_sub(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** mult (write, read, size) */
ZBool ZOpcode_mult(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** div (write, read, size) */
ZBool ZOpcode_div(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** mod (write, read, size) */
ZBool ZOpcode_mod(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** and (write, read, size) */
ZBool ZOpcode_and(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** or (write, read, size) */
ZBool ZOpcode_or(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** not (write) */
ZBool ZOpcode_not(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** xor (write, read, size) */
ZBool ZOpcode_xor(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** lshf (write, read, size) */
ZBool ZOpcode_lshf(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** rshf (write, read, size) */
ZBool ZOpcode_rshf(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** hash (write, read, size) */
ZBool ZOpcode_hash(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** int (write, read, size) */
ZBool ZOpcode_int(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** uint (write, read, size) */
ZBool ZOpcode_uint(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** flt (write, read, size) */
ZBool ZOpcode_flt(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}

/** ffi (read, size) */
ZBool ZOpcode_ffi(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
}
