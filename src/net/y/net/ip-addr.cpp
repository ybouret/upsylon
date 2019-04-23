
#include "y/net/ip-addr.hpp"
#include "y/memory/io.hpp"
#include "y/code/utils.hpp"

#include <iostream>

namespace upsylon
{
    namespace net
    {
        
        ip_address:: ~ip_address() throw()
        {
        }
        
        ip_address:: ip_address( void *data, const unsigned port_offset ) throw() :
        port( *memory::io::cast<net16_t>(data,port_offset) )
        {
        }
        
        
        ip_data<v4>::~ip_data() throw()
        {
            memset( &sa, 0, sizeof(sa) );
        }
        
        ip_data<v4>:: ip_data(const ip_addr_value value) throw() :
        sa(),
        addr( *memory::io::__force<net32_t>( &(sa.sin_addr) ) )
        {
            memset( &sa, 0, sizeof(sa) );
            sa.sin_family = AF_INET;
            _(value);
        }
        
        void ip_data<v4>::_(const ip_addr_value value) throw()
        {
            static const uint32_t __ip_addr_none = INADDR_NONE;
            static const uint32_t __ip_addr_any  = INADDR_ANY;
            static const uint32_t __ip_addr_lbck = INADDR_LOOPBACK;
            
            switch( value )
            {
                case ip_addr_none:     addr = bswp( __ip_addr_none ); break;
                case ip_addr_any:      addr = bswp( __ip_addr_any  ); break;
                case ip_addr_loopback: addr = bswp( __ip_addr_lbck ); break;
            }
        }
        
        void ip_data<v4>:: output(std::ostream &os) const
        {
            uint32_t dw = bswp(addr);
            os
            << int((dw>>24) & 0xff) << '.'
            << int((dw>>24) & 0xff) << '.'
            << int((dw>>24) & 0xff) << '.'
            << int( dw      & 0xff) << '@' << bswp(sa.sin_port);
        }
        
        
        
        ip_data<v6>::~ip_data() throw()
        {
            memset( &sa, 0, sizeof(sa) );
        }
        
        ip_data<v6>:: ip_data(const ip_addr_value value) throw() :
        sa(),
        addr(  *memory::io::__force<net128_t>( &(sa.sin6_addr) ) )
        {
            assert(sizeof(sa.sin6_addr)==sizeof(net128_t));
            memset( &sa, 0, sizeof(sa) );
            sa.sin6_family = AF_INET;
            _(value);
        }
        
        void ip_data<v6>::_(const ip_addr_value value) throw()
        {
            static const uint8_t lbck[16] =
            {
                1,0,0,0,
                0,0,0,0,
                0,0,0,0,
                0,0,0,0}
            ;
            
            switch( value )
            {
                case ip_addr_none:     memset( & sa.sin6_addr, 0xff, 16 ); break;
                case ip_addr_any:      memset( & sa.sin6_addr, 0x00, 16 ); break;
                case ip_addr_loopback: memcpy( & sa.sin6_addr, lbck, 16 ); break;
            }
        }
        
        void ip_data<v6>:: output(std::ostream &os) const
        {
            const net128_t x        = bswp(addr);
            char           buff[64] = { 0 };
            const uint8_t *q        = x.h;
            
            memset(buff,0,sizeof(buff));
            
            size_t j=0;
            for(size_t i=0;i<8;++i)
            {
                if(q[0]!=0||q[1]!=0)
                {
                    for(size_t iter=0;iter<2;++iter)
                    {
                        const char *w = hexadecimal::lowercase[*(q++)];
                        buff[j++] = w[0];
                        buff[j++] = w[1];
                    }
                }
                else
                {
                    q+=2;
                }
                if(i<7) buff[j++] = ':';
            }
            std::cerr << buff << "->";
            for(size_t k=1;k<j;)
            {
                if(buff[k]==':' && buff[k+1] == ':' )
                {
                    memmove(&buff[k],&buff[k+1],(j--)-k);
                }
                else
                {
                    ++k;
                }
            }
            
            os << buff << '@' << bswp(sa.sin6_port);
        }
        
    }
}
