// .c
// Z Program Class
// by Kyle Furey

#include <ZLang.h>

#ifdef ZLANG_SIGINT

/** A pointer to the last opened Z program. This is solely used for SIGINT. */
ZProgram *ZProgram_instance = NULL;

/** Z program SIGINT handler. */
void ZProgram_kill(int code) {
    printf("\n");
    Zlog("SIGINT.");
    ZProgram_delete(ZProgram_instance);
    signal(SIGINT, NULL);
    raise(SIGINT);
}

#endif

/** Returns the time in milliseconds. */
ZULong ZTime(ZUInt offsetMs) {
    clock_t ticks = clock();
    ZULong ms = (ZULong) ((ticks * 1000) / CLOCKS_PER_SEC);
    return ms + offsetMs;
}

/** Initializes a new Z program. */
ZBool ZProgram_new(ZProgram *self, ZString path, ZUInt argc, const ZString argv[]) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(path != NULL, "<path> was NULL!");
    Zassert(argv != NULL, "<argv> was NULL!");
    Zassert(argv[argc] == NULL, "<argv> was not null-terminated!");
    ZLong length = (ZLong) strlen(path);
    if (length <= 0) {
        Zerror("Invalid path!");
        return false;
    }
    for (--length; length >= 0; --length) {
        if (path[length] == '.') {
            ZString ext = path + length + 1;
            if (tolower((ZByte) ext[0]) == 'z' &&
                tolower((ZByte) ext[1]) == 'a' &&
                tolower((ZByte) ext[2]) == 'c') {
                break;
            }
            Zerror("Invalid executable file extension!");
            return false;
        }
        if (length <= 0) {
            Zerror("Cannot execute a directory!");
            return false;
        }
    }
    if (!ZVector_new(&self->coroutines, ZLANG_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize coroutine vector!");
        return false;
    }
    ZCoroutine *main = (ZCoroutine *) malloc(sizeof(ZCoroutine));
    if (main == NULL) {
        Zerror("Could not allocate main coroutine!");
        ZVector_delete(&self->coroutines);
        return false;
    }
    if (!ZCoroutine_newMain(main, argc, argv)) {
        Zerror("Could not initialize main coroutine!");
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    if (!ZVector_push(&self->coroutines, (ZULong) main)) {
        Zerror("Could not insert main coroutine!");
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    self->current = 0;
    self->next = 1;
    if (!ZVector_new(&self->files, ZLANG_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize fileStream vector!");
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    ZFileStream *zac = (ZFileStream *) malloc(sizeof(ZFileStream));
    if (zac == NULL) {
        Zerror("Could not allocate ZAC file stream!");
        ZVector_delete(&self->files);
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    if (!ZFileStream_new(zac, path, 0)) {
        Zerror("Could not initialize ZAC file stream!");
        free(zac);
        ZVector_delete(&self->files);
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    if (!ZVector_push(&self->files, (ZULong) zac)) {
        Zerror("Could not insert ZAC file stream!");
        ZFileStream_delete(zac);
        free(zac);
        ZVector_delete(&self->files);
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }
    if (!ZLibrary_new(&self->libc, "libc")) {
        Zerror("Could not initialize libc library!");
        ZFileStream_delete(zac);
        free(zac);
        ZVector_delete(&self->files);
        ZCoroutine_delete(main);
        free(main);
        ZVector_delete(&self->coroutines);
        return false;
    }

#ifdef ZLANG_SIGINT

    ZProgram_instance = self;
    signal(SIGINT, (__sighandler_t) ZProgram_kill);

#endif

    return true;
}

/** Starts a new coroutine in a Z program. */
ZBool ZProgram_startCoroutine(
    ZProgram *self,
    ZUInt handleStart,
    ZUInt argSize,
    ZUInt globalOffset
) {
    Zassert(self != NULL, "<self> was NULL!");
    ZCoroutine *coro = (ZCoroutine *) malloc(sizeof(ZCoroutine));
    if (coro == NULL) {
        Zerror("Could not allocate async coroutine!");
        return false;
    }
    ZCoroutine *parent = (ZCoroutine *) ZVector_get(&self->coroutines, self->current);
    if (!ZCoroutine_newAsync(
        coro,
        handleStart,
        argSize,
        parent,
        globalOffset,
        self->next
    )) {
        Zerror("Could not initialize async coroutine!");
        free(coro);
        return false;
    }
    if (self->next == self->coroutines.count) {
        if (!ZVector_push(&self->coroutines, (ZULong) coro)) {
            Zerror("Could not insert async coroutine!");
            ZCoroutine_delete(coro);
            free(coro);
            return false;
        }
        ++self->next;
    } else {
        ZVector_set(&self->coroutines, self->next++, (ZULong) coro);
        while (self->next < self->coroutines.count &&
               (ZCoroutine *) ZVector_get(&self->coroutines, self->next) != NULL) {
            ++self->next;
        }
    }
    return true;
}

/** Stops a coroutine by its index in a Z program. Only "fails" on stopping main. */
ZBool ZProgram_stopCoroutine(ZProgram *self, ZUInt index, ZBool dispatch) {
    Zassert(self != NULL, "<self> was NULL!");
    if (index == ZLANG_COROUTINE_MAIN) {
        return false;
    }
    if (index >= self->coroutines.count) {
        return true;
    }
    ZCoroutine *coro = (ZCoroutine *) ZVector_get(&self->coroutines, index);
    if (coro == NULL) {
        return true;
    }
    ZVector_set(&self->coroutines, index, 0);
    if (index < self->next) {
        self->next = index;
    }
    if (dispatch) {
        ZCoroutine_dispatch(
            coro,
            self->coroutines.count,
            (ZCoroutine **) self->coroutines.array
        );
    }
    ZCoroutine_delete(coro);
    free(coro);
    return true;
}

/** Loads a .zlib file at the given path into the Z program. */
ZBool ZProgram_loadLibrary(ZProgram *self, ZString path) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(path != NULL, "<path> was NULL!");
    ZLong length = (ZLong) strlen(path);
    if (length <= 0) {
        Zerror("Invalid path!");
        return false;
    }
    for (--length; length >= 0; --length) {
        if (path[length] == '.') {
            ZString ext = path + length + 1;
            if (tolower((ZByte) ext[0]) == 'z' &&
                tolower((ZByte) ext[1]) == 'l' &&
                tolower((ZByte) ext[2]) == 'i' &&
                tolower((ZByte) ext[3]) == 'b') {
                break;
            }
            Zerror("Invalid library file extension!");
            return false;
        }
        if (length <= 0) {
            Zerror("Cannot execute a directory!");
            return false;
        }
    }
    ZFileStream *zlib = (ZFileStream *) malloc(sizeof(ZFileStream));
    if (zlib == NULL) {
        Zerror("Could not allocate ZLIB file stream!");
        return false;
    }
    ZFileStream *parent = (ZFileStream *) ZVector_get(
        &self->files,
        self->files.count - 1
    );
    ZULong globalOffset = parent->globalOffset + parent->fileSize;
    if (!ZFileStream_new(zlib, path, globalOffset)) {
        Zerror("Could not initialize ZLIB file stream!");
        free(zlib);
        return false;
    }
    if (!ZVector_push(&self->files, (ZULong) zlib)) {
        Zerror("Could not insert ZLIB file stream!");
        ZFileStream_delete(zlib);
        free(zlib);
        return false;
    }
    return true;
}

/** Binds a foreign function to the Z program. */
ZBool ZProgram_bind(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZUInt library
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    ZLibrary *lib;
    if (library == ZLANG_FFI_LIBC) {
        lib = &self->libc;
    } else if (library >= file->libraries.count) {
        lib = (ZLibrary *) ZVector_get(&file->libraries, library);
    } else {
        Zerror("Invalid FFI library index!");
        return false;
    }
    ZUInt length;
    if (!ZFileStream_nextArray(file, sizeof(ZUInt), (ZByte *) &length, coro)) {
        Zerror("Unable to read FFI binding name length!");
        return false;
    }
    ZChar *str = (ZChar *) malloc(length * sizeof(ZChar));
    if (str == NULL) {
        Zerror("Could not allocate FFI binding name!");
        return false;
    }
    if (!ZFileStream_nextArray(file, length, (ZByte *) &str, coro)) {
        Zerror("Unable to read FFI binding name!");
        free(str);
        return false;
    }
    ZCall call;
    if (!ZFileStream_nextArray(file, sizeof(ZCall), (ZByte *) &call, coro)) {
        Zerror("Unable to read FFI binding call!");
        free(str);
        return false;
    }
    ZType *returnType = ZFileStream_getType(file, call.returnType);
    if (returnType == NULL) {
        Zerror("FFI binding has invalid return type!");
        free(str);
        return false;
    }
    ZUInt argCount = call.varArgs == ZLANG_CALL_NO_VARADIC ? call.fixedArgs : call.fixedArgs + call.varArgs;
    ZUInt *argIndicies = (ZUInt *) malloc((argCount + 1) * sizeof(ZUInt));
    if (argIndicies == NULL) {
        Zerror("Could not allocate FFI binding argument indicies!");
        free(str);
        return false;
    }
    ZType **argTypes = (ZType **) malloc((argCount + 1) * sizeof(ZType *));
    if (argTypes == NULL) {
        Zerror("Could not allocate FFI binding arguments!");
        free(argIndicies);
        free(str);
        return false;
    }
    for (ZUInt i = 0; i < argCount; ++i) {
        ZType *type = ZFileStream_getType(file, argIndicies[i]);
        if (type == NULL) {
            Zerror("FFI binding has invalid return type!");
            free(argTypes);
            free(argIndicies);
            free(str);
            return false;
        }
        argTypes[i] = type;
    }
    free(argIndicies);
    if (!ZFileStream_nextArray(file, argCount * sizeof(ZUInt), (ZByte *) &argIndicies, coro)) {
        Zerror("Unable to read FFI binding arguments!");
        free(argTypes);
        free(str);
        return false;
    }
    if (!ZLibrary_bind(
        lib,
        str,
        call.abi,
        call.fixedArgs,
        call.varArgs,
        returnType,
        argTypes
    )) {
        Zerror("Invalid FFI binding!");
        free(argTypes);
        free(str);
        return false;
    }
    free(argTypes);
    free(str);
    return true;
}

/** Calls a foreign function from the Z program's libraries. */
ZBool ZProgram_call(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file,
    ZUInt library,
    ZUInt ffi
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(file != NULL, "<file> was NULL!");
    Zassert(coro != NULL, "<coro> was NULL!");
    ZLibrary *lib;
    if (library == ZLANG_FFI_LIBC) {
        lib = &self->libc;
    } else if (library >= file->libraries.count) {
        lib = (ZLibrary *) ZVector_get(&file->libraries, library);
    } else {
        Zerror("Invalid FFI library index!");
        return false;
    }
    if (!ZLibrary_call(lib, ffi, &coro->stack)) {
        Zerror("Invalid FFI function index!");
        return false;
    }
    return true;
}

/** Executes the next coroutine in a Z program. Returns whether the program can continue. */
ZBool ZProgram_step(ZProgram *self) {
    Zassert(self != NULL, "<self> was NULL!");
    ZCoroutine *coro;
    do {
        self->current = (self->current + 1) % self->coroutines.count;
        coro = (ZCoroutine *) ZVector_get(&self->coroutines, self->current);
        // NOTE: Can deadlock
    } while (
        coro == NULL ||
        coro->delayMs > ZTime(0) ||
        (coro->await > 0 && (ZCoroutine *) ZVector_get(&self->coroutines, coro->await) != NULL)
    );
    ZULong current = ZTime(0);
    ZULong time = ZTime(ZLANG_COROUTINE_DELAY_MS);
    while (current < time) {
        if (!ZOpcode_nextCode(self)) {
            return false;
        }
        coro = (ZCoroutine *) ZVector_get(&self->coroutines, self->current);
        if (
            coro == NULL ||
            coro->delayMs > current ||
            (coro->await > 0 && (ZCoroutine *) ZVector_get(&self->coroutines, coro->await) != NULL)
        ) {
            break;
        }
        current = ZTime(0);
    }
    return true;
}

/** Runs a Z program until completion. Returns the result of main. */
ZInt ZProgram_execute(ZProgram *self) {
    Zassert(self != NULL, "<self> was NULL!");
    while (ZProgram_step(self)) {
    }
    ZCoroutine *main = (ZCoroutine *) ZVector_get(&self->coroutines, 0);
    return *(ZInt *) main->stack.bottom;
}

/** Cleans up all memory owned by a Z program. */
void ZProgram_delete(ZProgram *self) {
    Zassert(self != NULL, "<self> was NULL!");
    ZLibrary_delete(&self->libc);
    ZUInt count = self->files.count;
    for (ZUInt i = 0; i < count; ++i) {
        ZFileStream *file = (ZFileStream *) ZVector_get(&self->files, i);
        ZFileStream_delete(file);
        free(file);
    }
    ZVector_delete(&self->files);
    count = self->coroutines.count;
    for (ZUInt i = 0; i < count; ++i) {
        ZCoroutine *coro = (ZCoroutine *) ZVector_get(&self->coroutines, i);
        if (coro != NULL) {
            ZCoroutine_delete(coro);
            free(coro);
        }
    }
    ZVector_delete(&self->coroutines);

#ifdef ZLANG_SIGINT

    ZProgram_instance = NULL;
    signal(SIGINT, NULL);

#endif
}
