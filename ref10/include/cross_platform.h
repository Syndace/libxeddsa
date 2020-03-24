#ifndef cross_platform_h
#define cross_platform_h

#if defined(_WIN32)
//  On Windows, the functions needs to explicitly be marked as import/export.
#   define IMPORT __declspec(dllimport)
#   define EXPORT __declspec(dllexport)
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
//  On Apple, Linux, UNIX or POSIX (using gcc) no such marking is required.
#   define IMPORT
#   define EXPORT
#else
#   error "Unsupported operating system (neither UNIX nor Windows)."
#endif

#ifdef BUILD
#   define INTERFACE EXPORT
#else
#   define INTERFACE IMPORT
#endif

#endif
