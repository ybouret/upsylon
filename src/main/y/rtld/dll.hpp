//! \file
#ifndef Y_RTLD_DLL_INCLUDED
#define Y_RTLD_DLL_INCLUDED 1

#include "y/rtld/dll.h"
#include "y/string.hpp"

namespace upsylon
{

    //! a dynamically linked library
    class dll
    {
    public:
        dll( const string &soname );     //!< setup
        dll( const char   *soname );     //!< setup
        virtual ~dll() throw();          //!< destructor
        dll( const dll &other ) throw(); //!< shared copy

        void *load( const string &symbol ) throw(); //!< address of any symbol

        //! wrapper to address of any symbol
        inline
        void *load(const char    *symbol) { const string _(symbol); return load(_); }

        //! hook a symbol to a function prototype
        template <typename CFUNCTION> inline
        CFUNCTION hook( const string &symbol ) throw()
        {
            union {
                void     *addr;
                CFUNCTION user;
            } ans = { load(symbol)  };
            return ans.user;
        }

        //! wrapper to hook a function
        template <typename CFUNCTION> inline
        CFUNCTION hook( const char *symbol) throw()
        {
            const string _(symbol); return hook<CFUNCTION>(_);
        }

        //! link a symbol to some data
        template <typename T> inline
        T *ptr(const string &symbol) throw()
        {
            return static_cast<T *>( load(symbol) );
        }

        //! link a symbol to some data
        template <typename T> inline
        T *ptr(const char *symbol)
        {
            const string _(symbol); return ptr<T>(_);
        }
        


    private:
        Y_DISABLE_ASSIGN(dll);
        void   *handle; //!< system handle
        size_t *pcount; //!< shared count
    };

}

#endif

