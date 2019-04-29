//! \file
#ifndef Y_NET_SOCKET_SET_INCLUDED
#define Y_NET_SOCKET_SET_INCLUDED 1

#include "y/net/socket/delay.hpp"
#include "y/net/socket/bsd.hpp"

namespace upsylon
{
    namespace net
    {


        //! wrapper for socket_set
        class socket_set : public net_object
        {
        public:

            static const size_t capacity = FD_SETSIZE;  //!< system settings

            explicit socket_set();                      //!< allocate memory
            virtual ~socket_set() throw();              //!< destruct
            void insert( const bsd_socket &s );         //!< insert two levels
            void remove( const bsd_socket &s ) throw(); //!< failsafe remove
            void free() throw();                        //!< remove all
            
            const size_t size;      //!< current number of sockets
            const size_t allocated; //!< allocated bytes

            size_t probe( socket_delay &d );

            bool is_readable(  const bsd_socket &s ) throw(); //!< check and clear flag
            bool is_writable(  const bsd_socket &s ) throw(); //!< check and clear flag
            bool is_exception( const bsd_socket &s ) throw(); //!< check and clear flag


        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_set);
            void        *workspace; //!< shared memory
            socket_type *sock;      //!< capacity socket_type
            fd_set      *ufd;       //!< user's fd
            fd_set      *wfd;       //!< writable  fd
            fd_set      *rfd;       //!< readable  fd
            fd_set      *xfd;       //!< exception fd
        };

    }
}

#endif

