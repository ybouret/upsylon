#include "y/net/tcp/cache.hpp"

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
        buffer( memory::global::instance().acquire( (size_t &)allocated) ),
        pool( static_cast<char *>(buffer)+s_offset, s_length )
        {
            assert(pool.capacity()==capacity);
        }

        tcp_cache_:: ~tcp_cache_() throw()
        {
            reset();
            memset(buffer,0,allocated);
            memory::global::location().release(buffer, (size_t&)allocated);

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
            char        *data = static_cast<char *>(buffer);
            const size_t nr   = cln.recv(data,capacity);
            if(nr)
            {
                for(size_t i=0;i<nr;++i)
                {
                    byte_node *node = pool.acquire();
                    node->code = data[i];
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
