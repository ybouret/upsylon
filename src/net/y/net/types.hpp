//! \file
#ifndef Y_NET_TYPES_INCLUDED
#define Y_NET_TYPES_INCLUDED 1

#include "y/os/endian.hpp"
#include <iosfwd>

namespace upsylon
{


    typedef uint8_t  net8_t;  //!< alias
    typedef uint16_t net16_t; //!< alias
    typedef uint32_t net32_t; //!< alias
    typedef uint64_t net64_t; //!< alias

    //! a 128 bits object
    class net128_t
    {
    public:
        net128_t() throw();
        ~net128_t() throw();
        net128_t( const net128_t & ) throw();
        net128_t &operator=( const net128_t & ) throw();

        net128_t( const uint8_t buf[16] ) throw();

        uint8_t h[16];
        friend std::ostream & operator<< ( std::ostream &, const net128_t & );

    };

    namespace net
    {
        //! swap network byte order 16 bits
        inline uint16_t swap_nbo(const uint16_t x) throw()
        {
            return Y_SWAP_BE16(x);
        }

        //! swap network byte order 32 bits
        inline uint32_t swap_nbo(const uint32_t x) throw()
        {
            return Y_SWAP_BE32(x);
        }

        //! swap network byte order 64 bits
        inline uint64_t swap_nbo(const uint64_t x) throw()
        {
            return Y_SWAP_BE64(x);
        }
    }
}

#endif
