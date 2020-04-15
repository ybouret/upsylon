
//! \file

#ifndef Y_BE_ADDRESS_INCLUDED
#define Y_BE_ADDRESS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    
    //! Big Endian address
    class BEaddress
    {
    public:
        static const size_t size = sizeof(void*); //!< bytes from &data
        static void *BE(void *) throw();          //!< swap BE address
        ~BEaddress() throw();                     //!< cleanup
        BEaddress(const BEaddress&) throw();      //!< copy
        
        //! setup from address on one object
        template <typename T>
        inline BEaddress(const T &args) :
        data(  BE( (void*)&args ) )
        {
        }
        
        
        const void * const data;  //!< address, stored in BE format
        
        //! retrieve original address
        template <typename T>
        inline const T *as() const throw() { return static_cast<const T*>( BE((void*)data) ); }
        
    private:
        Y_DISABLE_ASSIGN(BEaddress);
    };
    
    
}

#endif

