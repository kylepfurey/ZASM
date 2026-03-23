// .c
// ZASM Runtime Execution Function
// by Kyle Furey

#include <ZASM.h>

/** Executes a .zac file at the given path, returning 0 or an error code from the program. */
ZInt Zexecute(ZString path, ZUInt argc, const ZString argv[]) {
    Zassert(path != NULL, "<path> was NULL!");
    Zassert(argv != NULL, "<argv> was NULL!");
    Zassert(argv[argc] == NULL, "<argv> was not null-terminated!");
    ZProgram program;
    if (!ZProgram_new(&program, path, argc, argv)) {
        Zerror("Failed to start program!");
        return ZASM_ERROR;
    }
    ZInt result = ZProgram_execute(&program);
    ZProgram_delete(&program);
    return result;
}
