
#include "y/rtld/rtld.hpp"
#include "y/type/aliasing.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/type/aliasing.hpp"
#include "y/associative/be-key.hpp"

#include "y/exceptions.hpp"
#if !defined(Y_WIN)
#include <dlfcn.h>
#endif

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
