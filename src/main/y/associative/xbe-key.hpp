
//! \file

#ifndef Y_XBE_KEY_INCLUDED
#define Y_XBE_KEY_INCLUDED 1

#include "y/os/xbe-address.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon {
    
    //! eXtended Big Endian Address to key
    template <typename T>
    class xbe_key : public memory::ro_buffer
    {
    public:
        Y_DECL_ARGS(T,type);
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~xbe_key() throw() {}
        
        //! copy
        inline xbe_key(const xbe_key &_) throw() : xaddr(_.xaddr)
        {
        }
        
        //! setup
        template <typename ARGS> inline
        xbe_key(const ARGS &args, param_type user) : memory::ro_buffer(), xaddr(args,user)
        {
        }
        
        //______________________________________________________________________
        //
        // memory buffer interface
        //______________________________________________________________________
        virtual const void *ro()     const throw() { return &xaddr; }
        virtual size_t      length() const throw() { return  xaddr.size; }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! get original data
        template <typename U> inline
        const U &as() const
        {
            return * xaddr.template as<U>();
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const XBEaddress<T> xaddr;
        
    private:
        Y_DISABLE_ASSIGN(xbe_key);
    };
    
}

#endif

