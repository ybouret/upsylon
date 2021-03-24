//! \file
#ifndef Y_RTLD_DLL_H_INCLUDED
#define Y_RTLD_DLL_H_INCLUDED 1

#include "y/os/setup.h"

#if defined(__cplusplus)
#define Y_DLL_EXTERN() extern "C" {
#define Y_DLL_FINISH() }
#else
#define Y_DLL_EXTERN()
#define Y_DLL_FINISH()
#endif


#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define Y_DLL_API 
#define Y_EXPORT  __declspec(dllexport)
#endif

#if defined(Y_BSD)
#define Y_DLL_API
#define Y_EXPORT
#endif




//______________________________________________________________________________
//
// Specific Module Init/Quit
//______________________________________________________________________________

#define Y_DLL_PRIVATE() static int y_dll_flags = 0


#if defined(Y_BSD)

#define Y_DLL_SETUP(ON_INIT,ON_QUIT)                      \
__attribute__((constructor)) void OnInit() { y_dll_flags=0; ON_INIT(); } \
__attribute__((destructor))  void OnQuit() { y_dll_flags=0; ON_QUIT(); }

#endif

#if defined(Y_WIN)

#define Y_DLL_SETUP(ON_INIT,ON_QUIT)  \
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID)\
{\
(void)hinstDLL;\
switch( fdwReason )\
{\
case DLL_PROCESS_ATTACH:  y_dll_flags=0; ON_INIT(); break;\
case DLL_THREAD_ATTACH:   y_dll_flags=1; ON_INIT(); break;\
case DLL_THREAD_DETACH:   y_dll_flags=1; ON_QUIT(); break;\
case DLL_PROCESS_DETACH:  y_dll_flags=0; ON_QUIT(); break;\
}\
return TRUE;\
}

#endif


#endif


