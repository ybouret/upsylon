#include "y/net/socket/hasher.hpp"

namespace upsylon
{
    namespace net
    {

        socket_hasher:: ~socket_hasher() throw() {}
        
        socket_hasher::  socket_hasher() throw() {}

        size_t socket_hasher:: operator()( const size_t k ) throw()
        {
            return k;
        }


    }
}

