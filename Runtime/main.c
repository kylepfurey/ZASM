// .c
// ZASM Runtime Entry Point
// by Kyle Furey

#include <ZASM.h>

/** Executes a ZASM program with arguments. */
int main(int argc, char *argv[]) {
    Zassert(argv != NULL, "<argv> was NULL!");
    Zassert(argv[argc] == NULL, "<argv> was not null-terminated!");
    if (argc <= 0) {
        Zerror("No file was given!");
        return ZASM_ERROR;
    }
    return Zexecute(
        (ZString) (argc == 1 ? argv[0] : argv[1]),
        (ZUInt) (argc - 1),
        (const ZString *) (argv + 1)
    );
}
