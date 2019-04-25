
#include "y/net/tcp/stream.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_istream:: ~tcp_istream() throw()
        {
        }

        tcp_istream:: tcp_istream( const tcp_link &conn ) :
        link( conn ),
        cache(),
        zbulk()
        {
            while( zbulk.size < bufsiz ) zbulk.store( new byte_node(0) );
        }

        bool tcp_istream:: query( char &C )
        {
            if(cache.size<=0 && !load())
            {
                return false;
            }
            else
            {
                C = cache.head->data;
                zbulk.store( cache.pop_front() );
                return true;
            }

        }

        void tcp_istream:: store(char C)
        {
            if(zbulk.size<=0) throw upsylon::exception("tcp_istream(%s): unable to store char!", (**link).text() );
            cache.push_front( zbulk.query() );
            cache.head->data = C;
        }

        bool tcp_istream:: load()
        {
            assert(0     ==cache.size);
            assert(bufsiz==zbulk.size);

            uint8_t       buffer[ bufsiz ];
            const size_t  nr = link->recv(buffer,bufsiz); assert(nr<=bufsiz);

            for(size_t i=0;i<nr;++i)
            {
                cache.push_back( zbulk.query() );
                cache.tail->data = buffer[i];
            }

            return cache.size>0;
        }

    }

}
