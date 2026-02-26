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
    ZOPCODE_load = 'L', // load (read)
    ZOPCODE_lib = 'l', // lib (read)
    ZOPCODE_read = 'R', // read (write, read, size)
    ZOPCODE_push = 'P', // push (read)
    ZOPCODE_pop = 'p', // pop (read)
    ZOPCODE_move = 'M', // move (write, read, size)
    ZOPCODE_set = 'S', // set (write, read, size)
    ZOPCODE_get = '{', // get (write, read, size)
    ZOPCODE_put = '}', // put (write, read, size)
    ZOPCODE_ptr = '@', // ptr (write, read)
    ZOPCODE_jump = 'J', // jump (read)
    ZOPCODE_hop = 'j', // hop (read)
    ZOPCODE_call = 'C', // call (read)
    ZOPCODE_ret = 'r', // ret (read)
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
    ZOPCODE_ffi = 'F', // ffi (read)
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

#endif // ZLANG_OPCODE_H
