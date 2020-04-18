

//! \file

#ifndef Y_XBE_ADDRESS_INCLUDED
#define Y_XBE_ADDRESS_INCLUDED 1

#include "y/os/be-address.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    
    
    //! eXtended Big Endian address
    template <typename T>
    class XBEaddress
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);
        class layout
        {
        public:
            const void  *addr;
            mutable_type attr;
            inline layout(const void *p, const_type &u ) throw() :
            addr(BEaddress::BE((void*)p)),
            attr(u)
            {
            }
            
            inline layout(const layout &_) throw() : addr(_.addr), attr(_.attr)
            {
            }
            
            inline ~layout() throw() { addr=0; }
            
        private:
            Y_DISABLE_ASSIGN(layout);
        };
        static const size_t size = sizeof(layout); //!< bytes
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! cleanup
        inline ~XBEaddress() throw() {}
        
        //! copy
        inline  XBEaddress(const XBEaddress &_) throw() : data(_.data) { }
        
        //! setup from address on one object
        template <typename ARGS>
        inline XBEaddress(const ARGS &args, param_type user) : data( &args, user) { }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! retrieve original address
        template <typename U>
        inline const U *as() const throw() { return static_cast<const U*>( BEaddress::BE((void*)data.addr) ); }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        
        const layout data;

    private:
        Y_DISABLE_ASSIGN(XBEaddress);
        
    };
    
    
}

#endif

