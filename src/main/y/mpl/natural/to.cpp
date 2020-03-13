#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace mpl {

        void natural:: throw_cast_overflow(const char *when) const
        {
            throw libc::exception( ERANGE, "mpn.cast overflow %s", (when ? when : "!") );
        }



        template <>
        bool natural::to<uint8_t>( uint8_t &u ) const throw()
        {
            switch(bytes)
            {
                case 0:  u = 0;      return true;
                case 1:  u = byte[0]; return true;
                default: break;
            }
            return false;
        }

#define B16(I) ( uint16_t(byte[I]) << (8*I) )

        template <>
        bool natural::to<uint16_t>( uint16_t &u ) const throw()
        {
            switch(bytes)
            {
                case 0:  u = 0;                          return true;
                case 1:  u = byte[0];                    return true;
                case 2:  u = B16(1) | uint16_t(byte[0]); return true;
                default: break;
            }
            return false;
        }
#undef B16

#define B32(I) ( uint32_t(byte[I]) << (8*I) )
        template <>
        bool natural::to<uint32_t>( uint32_t &u ) const throw()
        {
            switch(bytes)
            {
                case 0: u = 0;                                            return true;
                case 1: u = byte[0];                                      return true;
                case 2: u = B32(1) | uint32_t(byte[0]);                   return true;
                case 3: u = B32(2) | B32(1) | uint32_t(byte[0]);          return true;
                case 4: u = B32(3) | B32(2) | B32(1) | uint32_t(byte[0]); return true;
                default: break;
            }
            return false;
        }
#undef B32

#define B64(I) ( uint64_t(byte[I]) << (8*I) )
        template <>
        bool natural::to<uint64_t>( uint64_t &u ) const throw()
        {
            switch(bytes)
            {
                case 0: u = 0;                                                                                return true;
                case 1: u = byte[0];                                                                          return true;
                case 2: u = B64(1) | uint64_t(byte[0]);                                                       return true;
                case 3: u = B64(2) | B64(1) | uint64_t(byte[0]);                                              return true;
                case 4: u = B64(3) | B64(2) | B64(1) | uint64_t(byte[0]);                                     return true;
                case 5: u = B64(4) | B64(3) | B64(2) | B64(1) | uint64_t(byte[0]);                            return true;
                case 6: u = B64(5) | B64(4) | B64(3) | B64(2) | B64(1) | uint64_t(byte[0]);                   return true;
                case 7: u = B64(6) | B64(5) | B64(4) | B64(3) | B64(2) | B64(1) | uint64_t(byte[0]);          return true;
                case 8: u = B64(7) | B64(6) | B64(5) | B64(4) | B64(3) | B64(2) | B64(1) | uint64_t(byte[0]); return true;
                default: break;
            }
            return false;
        }
#undef B64


    }

}

