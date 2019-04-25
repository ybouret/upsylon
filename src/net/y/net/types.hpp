//! \file
#ifndef Y_NET_TYPES_INCLUDED
#define Y_NET_TYPES_INCLUDED 1

#include "y/os/endian.hpp"
#include "y/exception.hpp"
#include "y/net/sys.hpp"
#include "y/ptr/counted.hpp"

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
        
        void            reverse() throw();                           //!< reverse for byte swapping interface
        uint8_t       & operator[](const size_t indx) throw();       //!< access
        const uint8_t & operator[](const size_t indx) const throw(); //!< access, const


        //! display
        friend std::ostream & operator<< ( std::ostream &, const net128_t & );
    };

    namespace net
    {
        //! swap network byte order 16 bits
        inline uint8_t bswp(const uint8_t x) throw()
        {
            return x;
        }

        //! swap network byte order 16 bits
        inline uint16_t bswp(const uint16_t x) throw()
        {
            return Y_SWAP_BE16(x);
        }

        //! swap network byte order 32 bits
        inline uint32_t bswp(const uint32_t x) throw()
        {
            return Y_SWAP_BE32(x);
        }

        //! swap network byte order 64 bits
        inline uint64_t bswp(const uint64_t x) throw()
        {
            return Y_SWAP_BE64(x);
        }
        
        //! swap 128nits
        inline net128_t bswp(const net128_t &x) throw()
        {
            net128_t y(x);
            y.reverse();
            return y;
        }
        
        //! generic bswp
        template <typename T>
        inline T bswp_as( const T &x ) throw()
        {
            typedef typename unsigned_int<sizeof(T)>::type uint_T;
            union {
                T      item;
                uint_T bits;
            } alias = { x };
            alias.bits = bswp(alias.bits);
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

        //! return last error code, errno of WSAGetLastError()
        error_code get_last_error_code() throw();

        //! dedicated exception class
        class exception : public upsylon::exception
        {
        public:
            //! setup
            explicit exception(const error_code err,const char *fmt,...) throw() Y_PRINTF_CHECK(3,4);
            virtual ~exception() throw();           //!< default destructor
            exception( const exception & ) throw(); //!< copy constructor

            virtual const char *what() const throw(); //!< internal what_
            error_code          code() const throw(); //!< internal code_

        private:
            const error_code code_;
            char             what_[128-sizeof(error_code)];
            Y_DISABLE_ASSIGN(exception);
        };

    }

    namespace net
    {
        //! version[4|6]
        enum ip_version
        {
            v4, //!< for IPv4
            v6  //!< for IPv6
        };

        //! protocol[UDP|TCP]
        enum ip_protocol
        {
            tcp, //!< mark TCP
            udp  //!< mark UDP
        };

#if defined(Y_WIN)
        typedef SOCKET socket_type; //!< win32 sockets
        typedef int    sa_length_t; //!< for sizeof(sockaddr)
#endif

#if defined(Y_BSD)
        typedef int       socket_type; //!< bsd sockets
        typedef socklen_t sa_length_t; //!< for sizeof(sockaddr)
#endif

        extern const socket_type invalid_socket; //!< opaque invalid socket value
    }

    //! base class
    class net_object : public counted_object
    {
    public:
        virtual ~net_object() throw(); //!< setup
        explicit net_object() throw(); //!< destructor
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(net_object);
    };

}

#endif
