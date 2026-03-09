// .h
// Z Opcode Functions
// by Kyle Furey

#ifndef ZLANG_OPCODE_H
#define ZLANG_OPCODE_H

#include <Program.h>

/** Each enumerated operation code. */
enum {
    ZOPCODE_MAGIC = 'Z', // first and last byte of file
    ZOPCODE_exit = 'E', // exit (void)
    ZOPCODE_load = 'L', // load (size)
    ZOPCODE_lib = 'l', // lib (size)
    ZOPCODE_read = 'R', // read (write, read, size)
    ZOPCODE_push = 'P', // push (size)
    ZOPCODE_pop = 'p', // pop (size)
    ZOPCODE_move = 'M', // move (write, read, size)
    ZOPCODE_set = 'S', // set (write, read, size)
    ZOPCODE_get = '{', // get (write, read, size)
    ZOPCODE_put = '}', // put (write, read, size)
    ZOPCODE_ptr = '@', // ptr (write, read)
    ZOPCODE_jump = 'J', // jump (size)
    ZOPCODE_hop = 'j', // hop (size)
    ZOPCODE_call = 'C', // call (size)
    ZOPCODE_ret = 'r', // ret (size)
    ZOPCODE_addr = '$', // addr (write, read)
    ZOPCODE_eql = '=', // eql (write, read, size)
    ZOPCODE_grtr = 'G', // grtr (write, read, size)
    ZOPCODE_flip = '!', // flip (write)
    ZOPCODE_if = '?', // if (read, size)
    ZOPCODE_coro = 'A', // coro (write, read, size)
    ZOPCODE_bind = 'B', // bind (read)
    ZOPCODE_yld = 'Y', // yld (read)
    ZOPCODE_wait = 'W', // wait (read)
    ZOPCODE_cncl = 'X', // cncl (read)
    ZOPCODE_kill = 'Z', // kill (void)
    ZOPCODE_inc = 'i', // inc (write)
    ZOPCODE_dec = 'd', // dec (write)
    ZOPCODE_add = '+', // add (write, read, size)
    ZOPCODE_sub = '-', // sub (write, read, size)
    ZOPCODE_mult = '*', // mult (write, read, size)
    ZOPCODE_div = '/', // div (write, read, size)
    ZOPCODE_mod = '%', // mod (write, read, size)
    ZOPCODE_and = '&', // and (write, read, size)
    ZOPCODE_or = '|', // or (write, read, size)
    ZOPCODE_not = '~', // not (write)
    ZOPCODE_xor = '^', // xor (write, read, size)
    ZOPCODE_lshf = '<', // lshf (write, read, size)
    ZOPCODE_rshf = '>', // rshf (write, read, size)
    ZOPCODE_hash = '#', // hash (write, read, size)
    ZOPCODE_int = 's', // int (write, read, size)
    ZOPCODE_uint = 'u', // uint (write, read, size)
    ZOPCODE_flt = 'f', // flt (write, read, size)
    ZOPCODE_ffi = 'F', // ffi (read, size)
    ZOPCODE_TOTAL = 44, // <= 256
};

/** Each bitflag value for an operation. */
enum {
    ZOPCODE_FLAGS_WRITE_LITERAL = 1,
    ZOPCODE_FLAGS_READ_LITERAL = 2,
    ZOPCODE_FLAGS_SIZE_LITERAL = 4,
    ZOPCODE_FLAGS_SIGNED = 8,
    ZOPCODE_FLAGS_FLOAT = 16,
    ZOPCODE_FLAGS_WIDTH_ONE = 32,
    ZOPCODE_FLAGS_WIDTH_TWO = 64,
    ZOPCODE_FLAGS_WIDTH_FOUR = 128,
};

/** The table of Z operation codes to functions. */
extern ZBool (*const ZOpcode_TABLE[256])(ZProgram *, ZCoroutine *, ZFileStream *);

/** Executes a Z program's next operation code. */
ZLANG_API ZBool ZOpcode_nextCode(ZProgram *self);

/** exit (void) */
ZLANG_API ZBool ZOpcode_exit(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** load (size) */
ZLANG_API ZBool ZOpcode_load(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** lib (size) */
ZLANG_API ZBool ZOpcode_lib(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** read (write, read, size) */
ZLANG_API ZBool ZOpcode_read(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** push (size) */
ZLANG_API ZBool ZOpcode_push(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** pop (size) */
ZLANG_API ZBool ZOpcode_pop(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** move (write, read, size) */
ZLANG_API ZBool ZOpcode_move(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** set (write, read, size) */
ZLANG_API ZBool ZOpcode_set(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** get (write, read, size) */
ZLANG_API ZBool ZOpcode_get(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** put (write, read, size) */
ZLANG_API ZBool ZOpcode_put(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** ptr (write, read) */
ZLANG_API ZBool ZOpcode_ptr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** jump (size) */
ZLANG_API ZBool ZOpcode_jump(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** hop (size) */
ZLANG_API ZBool ZOpcode_hop(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** call (size) */
ZLANG_API ZBool ZOpcode_call(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** ret (size) */
ZLANG_API ZBool ZOpcode_ret(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** addr (write, read) */
ZLANG_API ZBool ZOpcode_addr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** eql (write, read, size) */
ZLANG_API ZBool ZOpcode_eql(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** grtr (write, read, size) */
ZLANG_API ZBool ZOpcode_grtr(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** flip (write) */
ZLANG_API ZBool ZOpcode_flip(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** if (read, size) */
ZLANG_API ZBool ZOpcode_if(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** coro (write, read, size) */
ZLANG_API ZBool ZOpcode_coro(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** bind (read) */
ZLANG_API ZBool ZOpcode_bind(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** yld (read) */
ZLANG_API ZBool ZOpcode_yld(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** wait (read) */
ZLANG_API ZBool ZOpcode_wait(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** cncl (read) */
ZLANG_API ZBool ZOpcode_cncl(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** kill (void) */
ZLANG_API ZBool ZOpcode_kill(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** inc (write) */
ZLANG_API ZBool ZOpcode_inc(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** dec (write) */
ZLANG_API ZBool ZOpcode_dec(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** add (write, read, size) */
ZLANG_API ZBool ZOpcode_add(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** sub (write, read, size) */
ZLANG_API ZBool ZOpcode_sub(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** mult (write, read, size) */
ZLANG_API ZBool ZOpcode_mult(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** div (write, read, size) */
ZLANG_API ZBool ZOpcode_div(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** mod (write, read, size) */
ZLANG_API ZBool ZOpcode_mod(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** and (write, read, size) */
ZLANG_API ZBool ZOpcode_and(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** or (write, read, size) */
ZLANG_API ZBool ZOpcode_or(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** not (write) */
ZLANG_API ZBool ZOpcode_not(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** xor (write, read, size) */
ZLANG_API ZBool ZOpcode_xor(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** lshf (write, read, size) */
ZLANG_API ZBool ZOpcode_lshf(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** rshf (write, read, size) */
ZLANG_API ZBool ZOpcode_rshf(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** hash (write, read, size) */
ZLANG_API ZBool ZOpcode_hash(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** int (write, read, size) */
ZLANG_API ZBool ZOpcode_int(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** uint (write, read, size) */
ZLANG_API ZBool ZOpcode_uint(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** flt (write, read, size) */
ZLANG_API ZBool ZOpcode_flt(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

/** ffi (read, size) */
ZLANG_API ZBool ZOpcode_ffi(
    ZProgram *self,
    ZCoroutine *coro,
    ZFileStream *file
);

#endif // ZLANG_OPCODE_H
