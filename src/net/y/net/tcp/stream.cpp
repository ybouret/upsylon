
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

        tcp_istream:: tcp_istream( const tcp_link &conn, const tcp_cache &shared ) :
        tcp_stream( conn ),
        cache(shared)
        {
            cache->reset();
        }

        bool tcp_istream:: query( char &C )
        {
            if(cache->size()<=0 && cache->load(*link)<=0 )
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


        tcp_ostream:: tcp_ostream( const tcp_link &conn ) throw() :
        tcp_stream( conn )
        {
        }

        void tcp_ostream:: flush() {}

        void tcp_ostream:: write(char C)
        {
            if( 1 != link->send_block( &C, 1) )
            {
                throw upsylon::exception("tcp_ostream(%s): disconnected!", (**link).text() );
            }
        }
    }

}

