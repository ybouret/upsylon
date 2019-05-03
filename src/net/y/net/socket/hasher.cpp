
#include "y/net/socket/hasher.hpp"
//#include "y/hashing/hash64.hpp"

namespace upsylon
{
    namespace net
    {

        socket_hasher:: ~socket_hasher() throw()       {
            //h.set();
        }
        
        socket_hasher::  socket_hasher() throw()  {
            //h.set();
            
        }

        size_t socket_hasher:: operator()( const socket_type &s ) throw()
        {
            // h.set();
            // h.run( &s, sizeof(socket_type) );
            // return h.key<size_t>();
            union
            {
                uint32_t    dw[2];
                size_t      sz;
                socket_type st;
            } item = { {0,0} }; //{ {0xffffffff,0xffffffff} };
            item.st = s;
            //hashing::hash64::OES(item.dw,item.dw+1);
            //std::cerr << "hashing: " << s << "->" << item.sz << std::endl;
            return item.sz;
        }


    }
}

