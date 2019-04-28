
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
        template <typename OUTPUT,typename U> inline OUTPUT chkbnd(const U &u) throw();

        //______________________________________________________________________
        //
        // INPUT=uint8_t
        //______________________________________________________________________
        template <> inline uint8_t  chkbnd<uint8_t, uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u8
        template <> inline uint16_t chkbnd<uint16_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u16
        template <> inline uint32_t chkbnd<uint32_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u32
        template <> inline uint64_t chkbnd<uint64_t,uint8_t>(const uint8_t &u) throw() { return u; } //!< u8->u64

        //! wrapper to let the compiler decide of the type
#define Y_CHKBND(OUTPUT,INPUT,VALUE) return ( (u>=static_cast<INPUT>(VALUE)) ? static_cast<OUTPUT>(VALUE) : static_cast<OUTPUT>(u) )

        template <> inline int8_t   chkbnd<int8_t,uint8_t>( const uint8_t &u) throw() { Y_CHKBND(int8_t,uint8_t,0x7f); } //!< u8->i8
        template <> inline int16_t  chkbnd<int16_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i16
        template <> inline int32_t  chkbnd<int32_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i32
        template <> inline int64_t  chkbnd<int64_t,uint8_t>(const uint8_t &u) throw() { return u; }                     //!< u8->i64



        //______________________________________________________________________
        //
        // INPUT=uint16_t
        //______________________________________________________________________
        template <> inline uint8_t  chkbnd<uint8_t, uint16_t>(const uint16_t &u) throw() { Y_CHKBND(uint8_t,uint16_t,0xff); } //!< u16->u8
        template <> inline uint16_t chkbnd<uint16_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u16
        template <> inline uint32_t chkbnd<uint32_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u32
        template <> inline uint64_t chkbnd<uint64_t,uint16_t>(const uint16_t &u) throw() { return u; }  //!< u16->u64

        template <> inline int8_t  chkbnd<int8_t, uint16_t>(const uint16_t &u) throw() { Y_CHKBND(int8_t,uint16_t,0x7f);    } //!< u16->i8
        template <> inline int16_t chkbnd<int16_t,uint16_t>(const uint16_t &u) throw() { Y_CHKBND(int16_t,uint16_t,0x7fff); } //!< u16->i16
        template <> inline int32_t chkbnd<int32_t,uint16_t>(const uint16_t &u) throw() { return u; }                         //!< u16->i32
        template <> inline int64_t chkbnd<int64_t,uint16_t>(const uint16_t &u) throw() { return u; }                         //!< u16->i64

        //______________________________________________________________________
        //
        // INPUT=uint32_t
        //______________________________________________________________________
        template <> inline uint8_t  chkbnd<uint8_t, uint32_t>(const uint32_t &u) throw() { Y_CHKBND(uint8_t,uint32_t,0xff);    } //!< u32->u8
        template <> inline uint16_t chkbnd<uint16_t,uint32_t>(const uint32_t &u) throw() { Y_CHKBND(uint16_t,uint32_t,0xffff); } //!< u32->u16
        template <> inline uint32_t chkbnd<uint32_t,uint32_t>(const uint32_t &u) throw() { return u; }                           //!< u32->u32
        template <> inline uint64_t chkbnd<uint64_t,uint32_t>(const uint32_t &u) throw() { return u; }                           //!< u32->u64
        
        template <> inline int8_t  chkbnd<int8_t, uint32_t>(const uint32_t &u) throw() { Y_CHKBND(int8_t,uint32_t,0x7f);        } //!< u32->i8
        template <> inline int16_t chkbnd<int16_t,uint32_t>(const uint32_t &u) throw() { Y_CHKBND(int16_t,uint32_t,0x7fff);     } //!< u32->i16
        template <> inline int32_t chkbnd<int32_t,uint32_t>(const uint32_t &u) throw() { Y_CHKBND(int32_t,uint32_t,0x7fffffff); } //!< u32->i32
        template <> inline int64_t chkbnd<int64_t,uint32_t>(const uint32_t &u) throw() { return u; }                              //!< u32->i64
        
        //______________________________________________________________________
        //
        // INPUT=uint64_t
        //______________________________________________________________________
        template <> inline uint8_t  chkbnd<uint8_t, uint64_t>(const uint64_t &u) throw() { Y_CHKBND(uint8_t,uint64_t,0xff);    }      //!< u64->u8
        template <> inline uint16_t chkbnd<uint16_t,uint64_t>(const uint64_t &u) throw() { Y_CHKBND(uint16_t,uint64_t,0xffff); }      //!< u64->u16
        template <> inline uint32_t chkbnd<uint32_t,uint64_t>(const uint64_t &u) throw() { Y_CHKBND(uint32_t,uint64_t,0xffffffff);  } //!< u64->u32
        template <> inline uint64_t chkbnd<uint64_t,uint64_t>(const uint64_t &u) throw() { return u; }                                //!< u64->u64
        
        template <> inline int8_t  chkbnd<int8_t, uint64_t>(const uint64_t &u) throw() { Y_CHKBND(int8_t,uint64_t,0x7f);        } //!< u64->i8
        template <> inline int16_t chkbnd<int16_t,uint64_t>(const uint64_t &u) throw() { Y_CHKBND(int16_t,uint64_t,0x7fff);     } //!< u64->i16
        template <> inline int32_t chkbnd<int32_t,uint64_t>(const uint64_t &u) throw() { Y_CHKBND(int32_t,uint64_t,0x7fffffff); } //!< u64->i32
        template <> inline int64_t chkbnd<int64_t,uint64_t>(const uint64_t &u) throw()
        {
            if(u>=Y_U64(0x7fffffffffffffff))
            {
                return Y_I64(0x7fffffffffffffff);
            }
            else
            {
                return static_cast<int64_t>(u);
            }
        } //!< u64->i64
        
        
    }
    
    //! return a positive bounded value of OUPUT based on INPUT type
    template <typename OUTPUT,typename INPUT>
    inline OUTPUT check_bound( INPUT x ) throw()
    {
        typedef typename limit_of<INPUT>::unsigned_type  UNSIGNED_TYPE;
        const UNSIGNED_TYPE ui = limit_of<INPUT>::positive(x);
        return core::chkbnd<OUTPUT,UNSIGNED_TYPE>(ui);
    }

    
}

#endif

