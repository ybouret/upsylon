#include "y/type/ints-display.hpp"
#include "y/exception.hpp"
#include <iostream>

namespace upsylon
{
    std::ostream & display_int:: sep(std::ostream &os , const char *separator)
    {
        if(separator) os << separator; else os << ' ';
        return os;
    }

    std::ostream & display_int::any(std::ostream &os,
                                    const void  *addr,
                                    const size_t size,
                                    const bool   sgnd)
    {
        typedef int64_t  s_type;
        typedef uint64_t u_type;

#define Y_DISPLAY_CASE(N,S,U) \
case N: if(sgnd) return os << s_type( *(const  S*)addr ); else return os << u_type( *(const U*)addr )
        
        assert(addr);
        switch(size)
        {
                Y_DISPLAY_CASE(1,int8_t, uint8_t );
                Y_DISPLAY_CASE(2,int16_t,uint16_t);
                Y_DISPLAY_CASE(4,int32_t,uint32_t);
                Y_DISPLAY_CASE(8,int64_t,uint64_t);

            default:
                throw exception("display_int::any(unhandled sizeof=%u)", unsigned(size) );

        }

        return os;
    }
}


