
#include "y/net/tcp/stream.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_stream:: ~tcp_stream() throw()
        {

        }

        tcp_stream:: tcp_stream( const tcp_link &conn ) throw() :
        link(conn)
        {
        }
        
    }

}




namespace upsylon
{
    namespace net
    {
        tcp_istream:: ~tcp_istream() throw()
        {
        }

        tcp_istream:: tcp_istream( const tcp_link &conn, const tcp_recv_cache &shared ) :
        tcp_stream( conn ),
        cache(shared)
        {
            cache->reset();
        }

        bool tcp_istream:: query( char &C )
        {
            if(cache->size()<=0 && cache->comm(*link)<=0 )
            {
                return false;
            }
            else
            {
                C = cache->getch();
                return true;
            }

        }

        void tcp_istream:: store(char C)
        {
            cache->putch(C);
        }
        

    }

}


namespace upsylon
{
    namespace net
    {

        tcp_ostream:: ~tcp_ostream() throw()
        {
        }


        tcp_ostream:: tcp_ostream( const tcp_link      &conn,
                                  const tcp_send_cache &oc) throw() :
        tcp_stream( conn ),
        cache(oc)
        {
            cache->reset();
        }

        void tcp_ostream:: flush()
        {
            while( cache->comm(*link) > 0  )
            {
            }
        }

        void tcp_ostream:: write(char C)
        {
            cache->push(C);
            if(cache->is_filled())
            {
                flush();
            }
        }
    }

}

