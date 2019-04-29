//! \file
#ifndef Y_NET_SOCKET_SET_INCLUDED
#define Y_NET_SOCKET_SET_INCLUDED 1

#include "y/net/socket/bsd.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace net
    {

        
        //! wrapper for socket_set
        class socket_set : public net_object
        {
        public:
            static const size_t capacity = FD_SETSIZE;
            explicit socket_set();
            virtual ~socket_set() throw();

            void insert( const bsd_socket &s );
            void free() throw();
            
            const size_t size;      //!< current number of fd
            const size_t allocated; //!< allocated bytes

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

