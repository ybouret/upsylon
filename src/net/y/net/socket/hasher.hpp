//! \file
#ifndef Y_NET_SOCKET_HASHER_INCLUDED
#define Y_NET_SOCKET_HASHER_INCLUDED 1

#include "y/net/types.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon
{
    namespace net
    {

        //! specific key hasher
        class socket_hasher : public net_object
        {
        public:
            explicit socket_hasher() throw(); //!< setup
            virtual ~socket_hasher() throw(); //!< cleanup

            //! return a hash key for a socket_type
            size_t operator()( const socket_type &s ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_hasher);
            hashing::fnv h;
        };

    }
}
#endif

