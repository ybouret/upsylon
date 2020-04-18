//! \file

#ifndef Y_BE_KEY_INCLUDED
#define Y_BE_KEY_INCLUDED 1

#include "y/os/be-address.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon {
 
    //! Big Endian Address to key
    class be_key : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~be_key() throw();       //!< cleanup
        be_key(const be_key &) throw();  //!< copy
        
        //! setup
        template <typename T> inline
        be_key(const T &args) : memory::ro_buffer(), address(args)
        {
        }
        
        //______________________________________________________________________
        //
        // memory buffer interface
        //______________________________________________________________________
        virtual const void *ro()     const throw(); //!< & address.addr
        virtual size_t      length() const throw(); //!<   BEaddress.size
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! get original data
        template <typename T> inline
        const T &as() const
        {
            assert( address.data );
            return * address.as<T>();
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const BEaddress address;
        
    private:
        Y_DISABLE_ASSIGN(be_key);
        void checkValidAddress(const void *) const;
    };
    
}

#endif

