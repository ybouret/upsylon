
#include "y/rtld.hpp"
#include "y/exceptions.hpp"
#if !defined(Y_WIN)
#include <dlfcn.h>
#endif

namespace upsylon
{
    void * rtld:: init(const string &soname)
    {
#if defined(Y_WIN)
        HMODULE h = ::LoadLibrary(*soname);
        if (!h)
        {
            throw win32::exception(::GetLastError(),"LoadLibrary(%s)",*soname);
        }
        return (void *)h;
#else
        void   *h = dlopen(*soname,RTLD_NOW);
        if( !h )
        {
            throw imported::exception("dlopen exception","%s", dlerror() );
        }
        return h;
#endif
    }

    void *rtld:: init(const char *soname)
    {
        const string _(soname);
        return init(_);
    }

    void rtld:: quit(void * &handle) throw()
    {
        assert(handle);
#if defined(Y_WIN)
        ::FreeLibrary((HMODULE)handle);
#else
        (void) dlclose(handle);
#endif
        handle=0;
    }

    void * rtld:: load(const void *handle, const string &symbol) throw()
    {
#if defined(Y_WIN)
        return (void *)(::GetProcAddress((HMODULE)handle, *symbol));
#else
        return dlsym( (void*)handle,*symbol);
#endif
    }

    void * rtld:: load(const void *handle, const char *symbol) throw()
    {
        const  string _(symbol);
        return load(handle,_);
    }

}

