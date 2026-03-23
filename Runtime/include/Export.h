// .h
// ZASM Export Macros
// by Kyle Furey

#ifndef ZASM_EXPORT_H
#define ZASM_EXPORT_H

/** The current ZASM runtime version. */
#define ZASM_VER 1.0

/** ZASM "home" environment variable. */
#define ZASM_HOME_VAR "ZASM_HOME"

#if defined(_WIN32) || defined(_WIN64)

/** Compiled to Windows. */
#define ZASM_WINDOWS

#elif defined(__APPLE__)

/** Compiled to MacOS. */
#define ZASM_MACOS

/** POSIX support. */
#define ZASM_POSIX

#else

/** Compiled to Linux. */
#define ZASM_LINUX

/** POSIX support. */
#define ZASM_POSIX

#endif

#ifdef ZASM_WINDOWS

/** File path separator. */
#define ZASM_PATH_SEPARATOR '\\'

#ifdef ZASM_EXPORTS

/** Windows export. */
#define ZASM_API __declspec(dllexport) inline

/** Defined if SIGINT is a valid way to kill a ZASM program. */
#define ZASM_SIGINT

#else

/** Windows import. */
#define ZASM_API __declspec(dllimport) inline

#endif

#else

/** File path separator. */
#define ZASM_PATH_SEPARATOR '/'

#ifdef ZASM_EXPORTS

/** MacOS / Linux export. */
#define ZASM_API __attribute__((visibility("default"))) inline

#else

/** Fallback. */
#define ZASM_API inline

/** Defined if SIGINT is a valid way to kill a ZASM program. */
#define ZASM_SIGINT

#endif

#endif

#ifdef _DEBUG

/** ZASM Runtime Debugging */
#define ZASM_DEBUG

#endif

#endif // ZASM_EXPORT_H
