
//! \file
#ifndef Y_TYPE_INTS_CHKBND_INCLUDED
#define Y_TYPE_INTS_CHKBND_INCLUDED 1

#include "y/type/ints.hpp"

#include <iostream>

namespace upsylon
{

    namespace core
    {
        //! for unsigned U=uint[8|16|32|64]
        template <typename OUTPUT,typename U> OUTPUT chkbnd(const U &u) throw();

        //______________________________________________________________________
        //
        // INPUT=uint8_t
        //______________________________________________________________________
        template <> uint8_t  chkbnd<uint8_t, uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u8
        template <> uint16_t chkbnd<uint16_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u16
        template <> uint32_t chkbnd<uint32_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u32
        template <> uint64_t chkbnd<uint64_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u64

#define Y_CHKBND(OUTPUT,INPUT,VALUE) return ( (u>=static_cast<INPUT>(VALUE)) ? static_cast<OUTPUT>(VALUE) : static_cast<OUTPUT>(u) )

        template <> int8_t   chkbnd<int8_t,uint8_t>( const uint8_t &u) throw() { Y_CHKBND(int8_t,uint8_t,127); } //!< u8->i8
        template <> int16_t  chkbnd<int16_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i16
        template <> int32_t  chkbnd<int32_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i32
        template <> int64_t  chkbnd<int64_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i64



        //______________________________________________________________________
        //
        // INPUT=uint16_t
        //______________________________________________________________________
        template <> uint8_t  chkbnd<uint8_t, uint16_t>(const uint16_t &u) throw() { Y_CHKBND(uint8_t,uint16_t,255); } //!< u16->u8
        template <> uint16_t chkbnd<uint16_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u16
        template <> uint32_t chkbnd<uint32_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u32
        template <> uint64_t chkbnd<uint64_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u64

        template <> int8_t  chkbnd<int8_t, uint16_t>(const uint16_t &u) throw() { Y_CHKBND(int8_t,uint16_t,127);    } //!< u16->u8
        template <> int16_t chkbnd<int16_t,uint16_t>(const uint16_t &u) throw() { Y_CHKBND(int16_t,uint16_t,32767); } //!< u16->u16
        template <> int32_t chkbnd<int32_t,uint16_t>(const uint16_t &u) throw() { return u; }                         //!< u16->u32
        template <> int64_t chkbnd<int64_t,uint16_t>(const uint16_t &u) throw() { return u; }                         //!< u16->u64


    }
    
    //! return a positive bounded value of OUPUT based on INPUT type
    template <typename OUTPUT,typename INPUT>
    inline OUTPUT check_bound( INPUT x ) throw()
    {
        typedef typename limit_of<INPUT>::unsigned_type  u_inp_t;
        //typedef typename limit_of<OUTPUT>::unsigned_type u_out_t;

        const u_inp_t ui = limit_of<INPUT>::positive(x);


        static const uint64_t  omax64 = static_cast<uint64_t>(limit_of<OUTPUT>::maximum);
        static const uint64_t  imax64 = static_cast<uint64_t>(limit_of<INPUT> ::maximum);
        static const uint64_t  vmax64 = (omax64<imax64) ? omax64 : imax64;
        static const INPUT     imax   = static_cast<INPUT>(vmax64);

        if( static_cast<uint64_t>(ui) > vmax64 )
        {
            return imax;
        }
        else
        {
            return x;
        }
    }

    
}

#endif

