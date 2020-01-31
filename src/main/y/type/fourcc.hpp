//! \file
#ifndef Y_FOURCC_INCLUDED
#define Y_FOURCC_INCLUDED

#include "y/os/platform.hpp"

//! byte
#define Y_FOURCC0(VALUE) ( uint32_t(uint8_t(VALUE)) )
//! shifted byte
#define Y_FOURCC1(VALUE) ( Y_FOURCC0(VALUE) << 0x08 )
//! shifted byte
#define Y_FOURCC2(VALUE) ( Y_FOURCC0(VALUE) << 0x10 )
//! shifted byte
#define Y_FOURCC3(VALUE) ( Y_FOURCC0(VALUE) << 0x18 )

//! Four Characters Code
#define Y_FOURCC(A,B,C,D) (Y_FOURCC3(A) | Y_FOURCC2(B) | Y_FOURCC1(C) | Y_FOURCC0(D))

//! extracting byte from a four char code
#define Y_FOURCC_AT(INDX,DWORD) ( uint8_t( uint32_t(DWORD) >> ( (3-(INDX)) << 3) ) )

namespace upsylon {

    //! thread unsafe conversion
    const char *fourcc_( const uint32_t dw ) throw();

    //! fourcc class helper
    class fourcc
    {
    public:
        const uint32_t uuid;    //!< user's unique identifier
        const char     clid[8]; //!< class identifier name

        fourcc( const uint32_t dw ) throw(); //!< setup
        fourcc( const fourcc  &   ) throw(); //!< copy
        virtual ~fourcc() throw();           //!< cleanup
        
    private:
        Y_DISABLE_ASSIGN(fourcc);
    };

}

#endif

