//! \file
#ifndef Y_RTLD_DLL_H_INCLUDED
#define Y_RTLD_DLL_H_INCLUDED 1

#include "y/os/setup.h"

#if defined(__cplusplus)
#define Y_DLL_EXTERN extern "C"
#else
#define Y_DLL_EXTERN
#endif


#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define Y_DLL_API  __cdecl
#define Y_DLL_SPEC __declspec(dllexport)
#endif

#if defined(Y_BSD)
#define Y_DLL_API
#define Y_DLL_SPEC
#endif

//! export function
#define Y_EXPORT Y_DLL_EXTERN Y_DLL_SPEC

//! export data
#define Y_SHARED Y_DLL_SPEC

//______________________________________________________________________________
//
// Specific Module Init/Quit
//______________________________________________________________________________

#if defined(Y_BSD)

#define Y_DLL_SETUP(ON_INIT,ON_QUIT) \
__attribute__((constructor)) void OnInit() { ON_INIT(); } \
__attribute__((destructor))  void OnQuit() { ON_QUIT(); }

#endif

#if defined(Y_WIN)

#define Y_DLL_SETUP(ON_INIT,ON_QUIT) \
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID)\
{\
(void)hinstDLL;\
switch( fdwReason )\
{\
case DLL_PROCESS_ATTACH: ON_INIT(); break;\
case DLL_THREAD_ATTACH:  ON_INIT(); break;\
case DLL_THREAD_DETACH:  ON_QUIT(); break;\
case DLL_PROCESS_DETACH: ON_QUIT(); break;\
}\
return TRUE;\
}

#endif


#endif


