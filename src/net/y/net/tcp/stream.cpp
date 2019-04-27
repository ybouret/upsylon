
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
        tcp_cache_:: tcp_cache_(const size_t n) :
        content(),
        capacity(n),
        s_offset( memory::align(capacity)       ),
        s_length( byte_slab::bytes_for(capacity)),
        allocated( s_offset + s_length ),
        buffer( static_cast<uint8_t *>( memory::global::instance().acquire( (size_t &)allocated) ) ),
        pool( &buffer[s_offset], s_length )
        {
            assert(pool.capacity()==capacity);
        }

        tcp_cache_:: ~tcp_cache_() throw()
        {
            reset();
            memory::global::location().release(*(void **)&buffer, (size_t&)allocated);
        }

        size_t tcp_cache_:: size() const throw() { return content.size; }


        void  tcp_cache_:: reset() throw()
        {
            while( content.size )
            {
                byte_node *node = content.pop_back();
                node->code = 0;
                pool.release(node);
            }
        }

        bool tcp_cache_:: load( const tcp_client &cln )
        {
            assert(content.size<=0);
            assert(pool.size()==capacity);
            const size_t nr = cln.recv(buffer,capacity);
            if(nr)
            {
                for(size_t i=0;i<nr;++i)
                {
                    byte_node *node = pool.acquire();
                    node->code = buffer[i];
                    content.push_back(node);
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        char tcp_cache_:: getch() throw()
        {
            assert(content.size>0);
            assert(content.size+pool.size()==capacity);
            const char ans = content.head->code;
            content.head->code=0;
            pool.release( content.pop_front() );
            assert(content.size+pool.size()==capacity);
            return ans;
        }

        void tcp_cache_:: putch(char C)
        {
            if( pool.size() <= 0 )
            {
                throw upsylon::exception("tcp_cache: unable to store char!");
            }
            byte_node *node = pool.acquire();
            node->code      = C;
            content.push_front(node);
        }

        tcp_cache_ * tcp_cache_new( const size_t n )
        {
            return new tcp_cache_(n);
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
            if(cache->size()<=0 && !cache->load(*link))
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

