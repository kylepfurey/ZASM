// .h
// Z Export Macros
// by Kyle Furey

#ifndef ZLANG_EXPORT_H
#define ZLANG_EXPORT_H

/** The current Z runtime version. */
#define ZLANG_VER 1.0

/** Z "home" environment variable. */
#define ZLANG_HOME_VAR "Z_HOME"

#if defined(_WIN32) || defined(_WIN64)

/** Compiled to Windows. */
#define ZLANG_WINDOWS

#elif defined(__APPLE__)

/** Compiled to MacOS. */
#define ZLANG_MACOS

/** POSIX support. */
#define ZLANG_POSIX

#else

/** Compiled to Linux. */
#define ZLANG_LINUX

/** POSIX support. */
#define ZLANG_POSIX

#endif

#ifdef ZLANG_WINDOWS

/** File path separator. */
#define ZLANG_PATH_SEPARATOR '\\'

#ifdef ZLANG_EXPORTS

/** Windows export. */
#define ZLANG_API __declspec(dllexport) inline

/** Defined if SIGINT is a valid way to kill a Z program. */
#define ZLANG_SIGINT

#else

/** Windows import. */
#define ZLANG_API __declspec(dllimport) inline

#endif

#else

/** File path separator. */
#define ZLANG_PATH_SEPARATOR '/'

#ifdef ZLANG_EXPORTS

/** MacOS / Linux export. */
#define ZLANG_API __attribute__((visibility("default"))) inline

#else

/** Fallback. */
#define ZLANG_API inline

/** Defined if SIGINT is a valid way to kill a Z program. */
#define ZLANG_SIGINT

#endif

#endif

#ifdef _DEBUG

/** Z Runtime Debugging */
#define ZLANG_DEBUG

#endif

#endif // ZLANG_EXPORT_H
