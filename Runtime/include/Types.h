// .h
// Z Type Declarations
// by Kyle Furey

#ifndef ZLANG_TYPES_H
#define ZLANG_TYPES_H

#include <External.h>
#include <Export.h>

/** A true or false boolean value. */
typedef bool ZBool;

/** An 8-bit signed integral number. */
typedef int8_t ZSByte;

/** An 8-bit unsigned integral number. */
typedef uint8_t ZByte;

/** A 16-bit signed integral number. */
typedef int16_t ZShort;

/** A 16-bit unsigned integral number. */
typedef uint16_t ZUShort;

/** A 32-bit signed integral number. */
typedef int32_t ZInt;

/** A 32-bit unsigned integral number. */
typedef uint32_t ZUInt;

/** A 64-bit signed integral number. */
typedef int64_t ZLong;

/** A 64-bit unsigned integral number. */
typedef uint64_t ZULong;

/** A 32-bit floating-point decimal number. */
typedef float ZFloat;

/** A 64-bit floating-point decimal number. */
typedef double ZDouble;

/** A 128-bit floating-point decimal number. */
typedef long double ZDecimal;

/** A byte representing a single symbol of text. */
typedef char ZChar;

/** A null-terminated sequence of characters representing text. */
typedef const ZChar *ZString;

/** A pointer to an anonymous function. */
typedef void (*ZFunc)(void);

#endif // ZLANG_TYPES_H
