// .c
// Z Dynamic Library Class
// by Kyle Furey

#include <ZLang.h>

/** Initializes a new dynamic library. "libc" loads the standard library. */
ZBool ZLibrary_new(ZLibrary *self, ZString name) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(name != NULL, "<name> was NULL!");
    if (!ZVector_new(&self->ffi, ZLANG_DEFAULT_CAPACITY)) {
        Zerror("Could not initialize dynamic library FFI vector!");
        return false;
    }
    ZULong nameLen = strlen(name);
    ZChar *buffer;

#ifdef ZLANG_WINDOWS

    // *.dll

    if (nameLen == 4 && strncmp(name, "libc", 4) == 0) {
        self->handle = LoadLibraryA("ucrtbase.dll");
        if (self->handle == NULL || GetProcAddress(self->handle, "printf") == NULL) {
            self->handle = LoadLibraryA("msvcrt.dll");
            if (self->handle == NULL || GetProcAddress(self->handle, "printf") == NULL) {
                Zerror("Could not locate libc!");
                ZVector_delete(&self->ffi);
                return false;
            }
        }
        return true;
    }
    buffer = (ZChar *) malloc(nameLen + 5); // [.][d][l][l][\0]
    if (buffer == NULL) {
        Zerror("Could not allocate buffer for dynamic library name!");
        ZVector_delete(&self->ffi);
        return false;
    }
    memcpy(buffer, name, nameLen);
    buffer[nameLen] = '.';
    buffer[nameLen + 1] = 'd';
    buffer[nameLen + 2] = 'l';
    buffer[nameLen + 3] = 'l';
    buffer[nameLen + 4] = '\0';
    self->handle = LoadLibraryA(buffer);
    if (self->handle == NULL) {
        free(buffer);
        ZString home = getenv(ZLANG_HOME_VAR);
        if (home == NULL) {
            Zerror("Dynamic library not found in directory!");
            ZVector_delete(&self->ffi);
            return false;
        }
        ZULong homeLen = strlen(home);
        buffer = (ZChar *) malloc(homeLen + nameLen + 6); // [/]...[.][d][l][l][\0]
        if (buffer == NULL) {
            Zerror("Could not allocate buffer for Z_HOME path + dynamic library name!");
            ZVector_delete(&self->ffi);
            return false;
        }
        memcpy(buffer, home, homeLen);
        buffer[homeLen] = ZLANG_PATH_SEPARATOR;
        memcpy(buffer + homeLen + 1, name, nameLen);
        ZULong len = homeLen + nameLen;
        buffer[++len] = '.';
        buffer[++len] = 'd';
        buffer[++len] = 'l';
        buffer[++len] = 'l';
        buffer[++len] = '\0';
        self->handle = LoadLibraryA(buffer);
        free(buffer);
        if (self->handle == NULL) {
            Zerror("Dynamic library not found in directory or Z_HOME!");
            ZVector_delete(&self->ffi);
            return false;
        }
    } else {
        free(buffer);
    }
    return true;

#else

#ifdef ZLANG_POSIX

#ifdef ZLANG_MACOS

    // lib*.dylib

    if (nameLen == 4 && strncmp(name, "libc", 4) == 0) {
        self->handle = dlopen("/usr/lib/libSystem.B.dylib", RTLD_NOW | RTLD_GLOBAL);
        if (self->handle == NULL || dlsym(self->handle, "printf") == NULL) {
            self->handle = dlopen("libSystem.B.dylib", RTLD_NOW | RTLD_GLOBAL);
            if (self->handle == NULL || dlsym(self->handle, "printf") == NULL) {
                Zerror("Could not locate libc!");
                ZVector_delete(&self->ffi);
                return false;
            }
        }
        return true;
    }
    buffer = (ZChar *) malloc(nameLen + 10); // [l][i][b]...[.][d][y][l][i][b][\0]
    if (buffer == NULL) {
        Zerror("Could not allocate buffer for dynamic library name!");
        ZVector_delete(&self->ffi);
        return false;
    }
    buffer[0] = 'l';
    buffer[1] = 'i';
    buffer[2] = 'b';
    memcpy(buffer + 3, name, nameLen);
    buffer[nameLen + 3] = '.';
    buffer[nameLen + 4] = 'd';
    buffer[nameLen + 5] = 'y';
    buffer[nameLen + 6] = 'l';
    buffer[nameLen + 7] = 'i';
    buffer[nameLen + 8] = 'b';
    buffer[nameLen + 9] = '\0';
    self->handle = dlopen(buffer, RTLD_NOW | RTLD_GLOBAL);
    if (self->handle == NULL) {
        free(buffer);
        ZString home = getenv(ZLANG_HOME_VAR);
        if (home == NULL) {
            Zerror("Dynamic library not found in directory!");
            ZVector_delete(&self->ffi);
            return false;
        }
        ZULong homeLen = strlen(home);
        buffer = (ZChar *) malloc(homeLen + nameLen + 11); // [/][l][i][b]...[.][d][y][l][i][b][\0]
        if (buffer == NULL) {
            Zerror("Could not allocate buffer for Z_HOME path + dynamic library name!");
            ZVector_delete(&self->ffi);
            return false;
        }
        memcpy(buffer, home, homeLen);
        buffer[homeLen] = ZLANG_PATH_SEPARATOR;
        buffer[homeLen + 1] = 'l';
        buffer[homeLen + 2] = 'i';
        buffer[homeLen + 3] = 'b';
        memcpy(buffer + homeLen + 4, name, nameLen);
        ZULong len = homeLen + nameLen + 3;
        buffer[++len] = '.';
        buffer[++len] = 'd';
        buffer[++len] = 'y';
        buffer[++len] = 'l';
        buffer[++len] = 'i';
        buffer[++len] = 'b';
        buffer[++len] = '\0';
        self->handle = dlopen(buffer, RTLD_NOW | RTLD_GLOBAL);
        free(buffer);
        if (self->handle == NULL) {
            Zerror("Dynamic library not found in directory or Z_HOME!");
            ZVector_delete(&self->ffi);
            return false;
        }
    } else {
        free(buffer);
    }
    return true;

#else

    // lib*.so

    if (nameLen == 4 && strncmp(name, "libc", 4) == 0) {
        self->handle = dlopen("libc.so.6", RTLD_NOW | RTLD_GLOBAL);
        if (self->handle == NULL || dlsym(self->handle, "printf") == NULL) {
            self->handle = dlopen("libc.so", RTLD_NOW | RTLD_GLOBAL);
            if (self->handle == NULL || dlsym(self->handle, "printf") == NULL) {
                Zerror("Could not locate libc!");
                ZVector_delete(&self->ffi);
                return false;
            }
        }
        return true;
    }
    buffer = (ZChar *) malloc(nameLen + 7); // [l][i][b]...[.][s][o][\0]
    if (buffer == NULL) {
        Zerror("Could not allocate buffer for dynamic library name!");
        ZVector_delete(&self->ffi);
        return false;
    }
    buffer[0] = 'l';
    buffer[1] = 'i';
    buffer[2] = 'b';
    memcpy(buffer + 3, name, nameLen);
    buffer[nameLen + 3] = '.';
    buffer[nameLen + 4] = 's';
    buffer[nameLen + 5] = 'o';
    buffer[nameLen + 6] = '\0';
    self->handle = dlopen(buffer, RTLD_NOW | RTLD_GLOBAL);
    if (self->handle == NULL) {
        free(buffer);
        ZString home = getenv(ZLANG_HOME_VAR);
        if (home == NULL) {
            Zerror("Dynamic library not found in directory!");
            ZVector_delete(&self->ffi);
            return false;
        }
        ZULong homeLen = strlen(home);
        buffer = (ZChar *) malloc(homeLen + nameLen + 8); // [/][l][i][b]...[.][s][o][\0]
        if (buffer == NULL) {
            Zerror("Could not allocate buffer for Z_HOME path + dynamic library name!");
            ZVector_delete(&self->ffi);
            return false;
        }
        memcpy(buffer, home, homeLen);
        buffer[homeLen] = ZLANG_PATH_SEPARATOR;
        buffer[homeLen + 1] = 'l';
        buffer[homeLen + 2] = 'i';
        buffer[homeLen + 3] = 'b';
        memcpy(buffer + homeLen + 4, name, nameLen);
        ZULong len = homeLen + nameLen + 3;
        buffer[++len] = '.';
        buffer[++len] = 's';
        buffer[++len] = 'o';
        buffer[++len] = '\0';
        self->handle = dlopen(buffer, RTLD_NOW | RTLD_GLOBAL);
        free(buffer);
        if (self->handle == NULL) {
            Zerror("Dynamic library not found in directory or Z_HOME!");
            ZVector_delete(&self->ffi);
            return false;
        }
    } else {
        free(buffer);
    }
    return true;

#endif

#endif

#endif

    Zerror("Cannot load with unknown dynamic library format!");
    ZVector_delete(&self->ffi);
    return false;
}

/** Returns a function pointer from a dynamic library via its name. */
ZFunc ZLibrary_find(ZLibrary *self, ZString name) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(name != NULL, "<name> was NULL!");
    Zassert(self->handle != NULL, "<self>'s handle was NULL!");

#ifdef ZLANG_WINDOWS

    return (ZFunc) GetProcAddress(self->handle, name);

#else

#ifdef ZLANG_POSIX

    return (ZFunc) dlsym(self->handle, name);

#endif

#endif

    Zerror("Cannot get function with unknown dynamic library format!");
    return NULL;
}

/** Binds a new foreign function to the library. */
ZBool ZLibrary_bind(
    ZLibrary *self,
    ZString name,
    ZUInt abi,
    ZUInt fixedArgs,
    ZUInt varArgs,
    ZType *returnType,
    ZType *argTypes[]
) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(name != NULL, "<name> was NULL!");
    Zassert(returnType != NULL, "<returnType> was NULL! Pass &ffi_type_void for void!");
    ZUInt argCount = varArgs == ZLANG_CALL_NO_VARADIC ? fixedArgs : fixedArgs + varArgs;
    Zassert(argTypes != NULL || argCount == 0, "<argTypes> was NULL when args > 0!");
    abi = abi == ZLANG_DEFAULT_ABI ? FFI_DEFAULT_ABI : abi;
    ZFFI *ffi = (ZFFI *) malloc(sizeof(ZFFI));
    if (ffi == NULL) {
        Zerror("Could not allocate FFI binding!");
        return false;
    }
    ZType **src = argTypes;
    argTypes = (ZType **) malloc((argCount + 1) * sizeof(ZType *));
    if (argTypes == NULL) {
        Zerror("Could not allocate array for FFI argument types!");
        free(ffi);
        return false;
    }
    memcpy(argTypes, src, argCount * sizeof(ZType *));
    argTypes[argCount] = NULL;
    ffi_status result;
    if (varArgs == ZLANG_CALL_NO_VARADIC) {
        result = ffi_prep_cif(
            &ffi->cif,
            (ffi_abi) abi,
            (unsigned int) fixedArgs,
            (ffi_type *) returnType,
            (ffi_type **) argTypes
        );
    } else {
        result = ffi_prep_cif_var(
            &ffi->cif,
            (ffi_abi) abi,
            (unsigned int) fixedArgs,
            (unsigned int) argCount,
            (ffi_type *) returnType,
            (ffi_type **) argTypes
        );
    }
    if (result != FFI_OK) {
        Zerror("Could not prepare FFI binding!");
        free(argTypes);
        free(ffi);
        return false;
    }
    ffi->cif.arg_types = (ffi_type **) argTypes;
    ffi->func = ZLibrary_find(self, name);
    if (ffi->func == NULL) {
        Zerror("Could not find FFI function!");
        free(argTypes);
        free(ffi);
        return false;
    }
    ZUInt *signature = (ZUInt *) malloc((argCount + 2) * sizeof(ZUInt));
    if (signature == NULL) {
        Zerror("Could not allocate FFI signature!");
        free(argTypes);
        free(ffi);
        return false;
    }
    signature[0] = (ZUInt) returnType->size;
    signature[1] = argCount;
    for (ZUInt i = 0; i < argCount; ++i) {
        signature[i + 2] = (ZUInt) argTypes[i]->size;
    }
    ffi->signature = signature;
    if (!ZVector_push(&self->ffi, (ZULong) ffi)) {
        Zerror("Could not insert FFI binding!");
        free(signature);
        free(argTypes);
        free(ffi);
        return false;
    }
    return true;
}

/** Invokes the foreign function stored in the library with the given index. */
ZBool ZLibrary_call(ZLibrary *self, ZUInt index, ZStack *stack) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(stack != NULL, "<stack> was NULL!");
    Zassert(self->handle != NULL, "<self>'s handle was NULL!");
    if (index >= self->ffi.count) {
        Zerror("Invalid FFI index!");
        return false;
    }
    ZFFI *ffi = (ZFFI *) ZVector_get(&self->ffi, index);
    Zassert(ffi->signature != NULL, "<ffi>'s signature array was NULL!");
    Zassert(ffi->func != NULL, "<ffi>'s func was NULL!");
    void *returnValue;
    if (ffi->signature[0] > 0) {
        returnValue = ZStack_peekTop(stack, ffi->signature[0], (ZULong) ffi->cif.rtype->size);
        if (returnValue == NULL) {
            Zerror("Could not retrieve return type offset from FFI call!");
            return false;
        }
    } else {
        returnValue = NULL;
    }
    void **argValues = (void **) malloc(ffi->signature[1] * sizeof(void *));
    if (argValues == NULL) {
        Zerror("Could not allocate argument type offsets for FFI call!");
        return false;
    }
    for (ZUInt i = 0; i < ffi->signature[1]; ++i) {
        argValues[i] = ZStack_peekTop(stack, ffi->signature[i + 2], (ZULong) ffi->cif.arg_types[i]->size);
        if (argValues[i] == NULL) {
            Zerror("Could not retrieve argument type offset from FFI call!");
            free(argValues);
            return false;
        }
    }
    ffi_call(&ffi->cif, ffi->func, returnValue, argValues);
    free(argValues);
    if (!ZStack_pop(stack, ffi->signature[ffi->signature[1] - 1])) {
        Zerror("Could not pop FFI arguments from stack!");
        return false;
    }
    return true;
}

/** Cleans up all memory owned by a dynamic library. */
void ZLibrary_delete(ZLibrary *self) {
    Zassert(self != NULL, "<self> was NULL!");
    Zassert(self->handle != NULL, "<self>'s handle was NULL!");

#ifdef ZLANG_WINDOWS

    FreeLibrary(self->handle);

#else

#ifdef ZLANG_POSIX

    dlclose(self->handle);

#endif

#endif

    self->handle = NULL;
    for (ZUInt i = 0; i < self->ffi.count; ++i) {
        ZFFI *ffi = (ZFFI *) ZVector_get(&self->ffi, i);
        free(ffi->signature);
        ffi->signature = NULL;
        free(ffi->cif.arg_types);
        ffi->cif = (ffi_cif){0};
        free(ffi);
    }
    ZVector_delete(&self->ffi);
}
