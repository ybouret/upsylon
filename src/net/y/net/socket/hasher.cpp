
#include "y/net/socket/hasher.hpp"

namespace upsylon
{
    namespace net
    {

        socket_hasher:: ~socket_hasher() throw()       { h.set(); }
        socket_hasher::  socket_hasher() throw() : h() { h.set(); }

        size_t socket_hasher:: operator()( const socket_type &s ) throw()
        {
            h.set();
            h.run( &s, sizeof(socket_type) );
            return h.key<size_t>();
        }


    }
}

