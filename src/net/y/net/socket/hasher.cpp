#include "y/net/socket/hasher.hpp"

namespace upsylon
{
    namespace net
    {

        socket_hasher:: ~socket_hasher() throw() { h.set(); }
        
        socket_hasher::  socket_hasher() throw() : h() { h.set(); }

        size_t socket_hasher:: operator()( const socket_type &s ) throw()
        {
            h.set();
            
            // initialize a field
            union
            {
                uint64_t    qw;
                socket_type st;
            } item = { 0 };
            
            // fill in the field
            item.st = s;
            
            // hash the field
            h.run_type(item.qw);
            return h.key<size_t>();
        }


    }
}

