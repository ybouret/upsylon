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
#include <windows.h>
//#define Y_DLL_API  WINAPI

#if defined(Y_IMPORT)
#define Y_DLL_SPEC __declspec(dllimport)
#else
#define Y_DLL_SPEC __declspec(dllexport)
#endif

#else

//#define Y_DLL_API  _cdecl
#define Y_DLL_SPEC

#endif

#define Y_DLL Y_DLL_EXTERN Y_DLL_SPEC


#endif


