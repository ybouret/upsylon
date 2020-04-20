

//! \file

#ifndef Y_XBE_ADDRESS_INCLUDED
#define Y_XBE_ADDRESS_INCLUDED 1

#include "y/os/endian.hpp"
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
        Y_DECL_ARGS(T,type);                                             //!< aliases
        typedef void *address_type;                                      //!< alias
        static  const size_t address_size = sizeof(address_type);        //!< bytes to store address
        static  const size_t size         = address_size + sizeof(type); //!< bytes
        
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! cleanup
        inline ~XBEaddress() throw() {}
        
        //! copy
        inline  XBEaddress(const XBEaddress &_) throw() : data()
        {
            memcpy((void*)data,_.data,sizeof(data));
        }
        
        //! setup from address on one object
        template <typename ARGS>
        inline XBEaddress(const ARGS &args, param_type user) : data()
        {
            store_be<address_type>( (address_type)&args, (void *)&data[0]            );
            store_be<mutable_type>(                user, (void*) &data[address_size] );
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! retrieve original address
        template <typename U> inline const U *as() const throw()
        { return static_cast<const U *>( query_be<address_type>( &data[0] ) ); }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const char data[size]; //!< compact workspace
        
    private:
        Y_DISABLE_ASSIGN(XBEaddress);
        
    };
    
    
}

#endif

