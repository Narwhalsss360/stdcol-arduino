#ifndef TestPlatform_h
#define TestPlatform_h

#if defined(ARDUINO)
#define plat_arduino "Arduino"
#define plat plat_arduino

#elif __has_include(<Windows.h>)
#define plat_windows "Windows"
#define plat plat_windows

#else
#define plat_unix "Unix"
#define plat plat_unix

#endif

#endif

