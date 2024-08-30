#ifndef stdcol_platform_h
#define stdcol_platform_h

#if defined(__has_include)
#if __has_include(<Arduino.h>)
#define stdcol_platform_arduino
#elif __has_include(<windows.h>)
#define stdcol_platform_windows
#elif __has_include(<unistd.h>)
#define stdcol_platform_linux
#else
#define stdcol_platform_from_macros
#endif //__has_include()
#else
#define stdcol_platform_from_macros
#endif //defined(__has_include)

#ifdef stdcol_platform_from_macros
#if defined(ARDUINO)
#define stdcol_platform_arduino
#elif defined(__APPLE__)
#define stdcol_platform_macos
#elif defined(__linux__)
#define stdcol_platform_linux
#elif defined(__MINGW32__)
#define stdcol_platform_windows
#else
#define stdcol_platform_generic
#endif //defined(ARDUINO)
#endif //stdcol_platform_from_macros

#if defined(stdcol_platform_arduino)
#define stdcol_platform_name "Arduino"

#ifndef ARDUINO_ARCH_RP2040
#define stdcol_nostl
#endif

#elif defined(stdcol_platform_macos)
#define stdcol_platform_name "MacOS"
#elif defined(stdcol_platform_linux)
#define stdcol_platform_name "Linux"
#elif defined(stdcol_platform_windows)
#define stdcol_platform_name "Windows"
#elif defined(stdcol_platform_generic)
#define stdcol_platform_name "Generic"
#define stdcol_nostl
#endif //defined(stdcol_platform_arduino)

#endif //stdcol_platform_h
