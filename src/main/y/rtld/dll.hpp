//! \file
#ifndef Y_RTLD_DLL_INCLUDED
#define Y_RTLD_DLL_INCLUDED 1

#if !defined(Y_IMPORT)
#define Y_IMPORT 1
#endif

#include "y/rtld/export.h"
#include "y/string.hpp"

namespace upsylon
{

    class dll
    {
    public:
        dll( const string &soname );
        dll( const char   *soname );
        virtual ~dll() throw();
        dll( const dll &other ) throw();

        void *load( const string &symbol ) throw();
        
        inline
        void *load(const char    *symbol) { const string _(symbol); return load(_); }

        template <typename CFUNCTION> inline
        CFUNCTION hook( const string &symbol ) throw()
        {
            union {
                void     *addr;
                CFUNCTION user;
            } ans = { load(symbol)  };
            return ans.user;
        }

        template <typename CFUNCTION> inline
        CFUNCTION hook( const char *symbol) throw()
        {
            const string _(symbol); return hook<CFUNCTION>(_);
        }

        template <typename T> inline
        T *ptr(const string &symbol) throw()
        {
            return static_cast<T *>( load(symbol) );
        }

        template <typename T> inline
        T *ptr(const char *symbol)
        {
            const string _(symbol); return ptr<T>(_);
        }
        


    private:
        Y_DISABLE_ASSIGN(dll);
        void   *handle;
        size_t *pcount;
    };

}

#endif

