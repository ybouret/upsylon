//! \file
#ifndef Y_RTLD_DLL_INCLUDED
#define Y_RTLD_DLL_INCLUDED 1

#include "y/rtld.hpp"
#include "y/rtld/dll.h"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! a dynamically linked library
    //
    //__________________________________________________________________________
    class dll
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup
        template <typename ID> inline
        explicit dll(const ID &soname ) : handle( rtld::init(soname) )
        {
        }
        
        //! cleanup
        virtual ~dll() throw();
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! load a symbol
        template <typename ID> inline
        void *load(const ID &symbol) const throw()
        {
            return rtld::load(handle,symbol);
        }
        
        template <typename CFUNCTION>
        struct hook
        {
            template <typename ID> static inline
            CFUNCTION load(const dll &so, const ID &id) throw()
            {
                union {
                    void      *addr;
                    CFUNCTION  proc;
                } alias = { so.load(id) };
                return alias.proc;
            }
        };
        
        template <typename T>
        struct data
        {
            template <typename ID> static inline
            T *load(const dll &so, const ID &id) throw()
            {
                return static_cast<T*>( so.load(id) );
            }
        };
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(dll);
        void *handle;
    };

}

#endif

