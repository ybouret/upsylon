#include "y/rtld/dll.hpp"
#include "y/exceptions.hpp"

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <window.h>
#else
#include <dlfcn.h>
#endif

namespace upsylon
{

    namespace
    {
        static inline void * __rtld_init( const char *id )
        {
            Y_GIANT_LOCK();
#if defined(Y_WIN)

#else
            void   *h = dlopen(id,RTLD_NOW);
            if( !h )
            {
                throw imported::exception("dlopen exception","%s", dlerror() );
            }
            return h;
#endif
        }

        static inline void __rtld_quit( void *h )
        {
            assert(h);
#if defined(Y_WIN)

#else
            (void) dlclose(h);
#endif

        }

    }

    dll:: ~dll() throw()
    {
        assert(pcount);
        assert(*pcount>0);
        if(--(*pcount)<=0)
        {
            __rtld_quit(handle);
            object::release1(pcount);
        }
    }



    dll:: dll( const string &soname ) :
    handle( NULL ),
    pcount( object::acquire1<size_t>() )
    {
        try
        {
            handle  = __rtld_init(*soname);
            *pcount = 1;
        }
        catch(...)
        {
            object::release1(pcount);
            throw;
        }
    }

    dll:: dll( const char *soname ) :
    handle( NULL ),
    pcount( object::acquire1<size_t>() )
    {
        try
        {
            handle  = __rtld_init(soname);
            *pcount = 1;
        }
        catch(...)
        {
            object::release1(pcount);
            throw;
        }
    }


    dll:: dll( const dll &other ) throw() :
    handle( other.handle ),
    pcount( other.pcount  )
    {
        ++(*pcount);
    }

    void * dll:: load( const string &symbol ) throw()
    {
#if defined(Y_WIN)

#else
        return dlsym(handle,*symbol);
#endif

    }
    

}
