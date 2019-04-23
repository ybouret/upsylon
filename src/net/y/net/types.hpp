//! \file
#ifndef Y_NET_TYPES_INCLUDED
#define Y_NET_TYPES_INCLUDED 1

#include "y/os/endian.hpp"
#include "y/exception.hpp"
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
        net128_t() throw();                              //!< set to 0
        ~net128_t() throw();                             //!< cleanup
        net128_t( const net128_t & ) throw();            //!< copy
        net128_t &operator=( const net128_t & ) throw(); //!< assign
        net128_t( const uint8_t buf[16] ) throw();       //!< manual setup

        uint8_t h[16]; //!< content

        //! display
        friend std::ostream & operator<< ( std::ostream &, const net128_t & );
    };

    namespace net
    {
        //! swap network byte order 16 bits
        inline uint8_t swap_nbo(const uint8_t x) throw()
        {
            return x;
        }

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

        template <typename T>
        inline T swap_nbo_as( const T &x ) throw()
        {
            typedef typename unsigned_int<sizeof(T)>::type uint_T;
            union {
                T      item;
                uint_T bits;
            } alias = { x };
            alias.bits = swap_nbo(alias.bits);
            return alias.item;
        }
    }

    namespace net
    {
#if defined(Y_BSD)
        typedef int error_code; //!< from errno
#endif

#if defined(Y_WIN)
        typedef int error_code; //!< from WSAGetLastError()
#endif
        class exception : public upsylon::exception
        {
        public:
            explicit exception(const error_code err,const char *fmt,...) throw() Y_PRINTF_CHECK(3,4);
            virtual ~exception() throw(); //!< default destructor
            exception( const exception & ) throw(); //!< copy constructor

            virtual const char *what() const throw(); //!< internal what_
            error_code          code() const throw(); //!< internal code_

        private:
            const error_code code_;
            char             what_[128-sizeof(error_code)];
        };

    }


}

#endif
