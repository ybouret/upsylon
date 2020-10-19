
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

#if 0

namespace upsylon
{

    class DLL
    {
    public:
        DLL(void *h) throw() : handle(h), shared(1) { }
        ~DLL() throw() { }
        DLL(const DLL &other) throw() : handle(other.handle), shared(other.shared) {}
        
        
        void  *handle;
        size_t shared;
        
        typedef suffix_tree<DLL> DB;
        
    private:
        Y_DISABLE_ASSIGN(DLL);
    };
    
    
    Y_SINGLETON_IMPL(rtld);
    
    namespace
    {
        uint64_t DLL_MEM[ Y_U64_FOR_ITEM(DLL::DB)];
        
        static inline DLL::DB * DLL_Link() throw()
        {
            return aliasing::as<DLL::DB>(DLL_MEM);
        }
        
        static DLL * DLL_Link_Search(void *handle) throw()
        {
            const be_key key(handle);
            return(DLL *)(DLL_Link()->search_by(key));
        }
        
        static void * DLL_Link_Init(const string &soname)
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
        
        static void DLL_Link_Quit(void *h) throw()
        {
            assert(h);
#if defined(Y_WIN)
            ::FreeLibrary((HMODULE)h);
#else
            (void) dlclose(h);
#endif
        }
        
        
    }
    
    rtld:: rtld()
    {
        Y_LOCK(access);
        memset(DLL_MEM,0,sizeof(DLL_MEM));
        new ( DLL_Link() ) DLL::DB();
    }
    
    rtld:: ~rtld() throw()
    {
        Y_LOCK(access);
        self_destruct( *DLL_Link() );
        memset(DLL_MEM,0,sizeof(DLL_MEM));
    }
    
    size_t rtld:: linked() const throw()
    {
        return DLL_Link()->entries();
    }
    
    
}


namespace upsylon
{
 
    void  * rtld:: link(const string &soname)
    {
        Y_GIANT_LOCK();
        void *handle = DLL_Link_Init(soname);
        DLL  *theDLL = DLL_Link_Search(handle);
        if(theDLL)
        {
            // already linked
            DLL_Link_Quit(handle);
        }
        else
        {
            // store newly linked
            try
            {
                const DLL newDLL(handle);
                
            }
            catch(...)
            {
                DLL_Link_Quit(handle);
                throw;
            }
        }
        return handle;
    }

}
#endif
