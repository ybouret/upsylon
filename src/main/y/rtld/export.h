//! \file
#ifndef Y_RTLD_EXPORT_INCLUDED
#define Y_RTLD_EXPORT_INCLUDED 1

#include "y/os/setup.h"

#if defined(__cplusplus)
#define Y_EXTERN extern "C"
#else
#define Y_EXTERN
#endif




#if defined(Y_WIN)
#include <windows.h>
#define Y_DLL_API  WINAPI

#if defined(Y_IMPORT)
#define Y_DLL_DECL __declspec(dllimport)
#else
#define Y_DLL_DECL __declspec(dllexport)
#endif

#else
#define Y_DLL_API
#define Y_DLL_DECL
#endif

#define Y_EXPORT Y_EXTERN Y_DLL_DECL


#endif


