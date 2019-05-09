//! \file
#ifndef Y_NET_SOCKET_ID_HASHER_INCLUDED
#define Y_NET_SOCKET_ID_HASHER_INCLUDED 1

#include "y/net/types.hpp"
#include "y/os/static-check.hpp"

namespace upsylon
{
    namespace net
    {
        //! key hasher for socket_id_t
        class socket_id_hasher
        {
        public:
            inline  socket_id_hasher() throw() {} //!< constructor, do nothing
            inline ~socket_id_hasher() throw() {} //!< destructor,  do nothing

            //! normally, socket_id_t fits in size_t...
            inline size_t operator()(const socket_id_t &sid) const throw()
            {
                Y_STATIC_CHECK( sizeof(size_t)>=sizeof(socket_id_t),socked_id_too_large);
                union { size_t key; socket_id_t sid; } alias = { 0 };
                alias.sid = sid;
                return alias.key;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_id_hasher);
        };
    }
}

#endif
