
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

    }

}

namespace upsylon
{
    namespace net
    {

        ip_data<v4>::~ip_data() throw()
        {
            memset( &sa, 0, sizeof(sa) );
        }
        
        ip_data<v4>:: ip_data(const ip_addr_value value) throw() :
        sa(),
        addr( *memory::io::__force<net32_t>( &(sa.sin_addr) ) ),
        hrb()
        {
            memset( &sa,  0, sizeof(sa)  );
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

        static inline void __add_dec( char * &buff, unsigned x ) throw()
        {
            assert(x<256);
            const unsigned c = x / 100;          assert(c<=2);
            const unsigned d = ( x-=c*100) / 10; assert(d<10);
            const unsigned u = ( x-=d*10);       assert(u<10);
            if(c) { *(buff++) = '0'+c;}
            if(d) { *(buff++) = '0'+d;}
            *(buff++) = '0'+u;
        }

        const char * ip_data<v4>:: hr() const throw()
        {
            char    *buff = & hrb.ch[0];
            uint32_t dw   = bswp(addr);

            hrb.clear();
            __add_dec(buff,unsigned((dw>>24) & 0xff)); *(buff++) = '.';
            __add_dec(buff,unsigned((dw>>16) & 0xff)); *(buff++) = '.';
            __add_dec(buff,unsigned((dw>>8)  & 0xff)); *(buff++) = '.';
            __add_dec(buff,unsigned( dw      & 0xff));

            return hrb.ch;
        }
        
    }

}

namespace upsylon
{
    namespace net
    {
        
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

        static inline void __add4bits( char * &buff, const uint16_t q) throw()
        {
            assert(q<16);
            if(q>0)
            {
                *(buff++) = * hexadecimal::lowercase_word[q];
            }
        }
        
        static inline void __add_hexa( char * &buff, const uint16_t w ) throw()
        {
            __add4bits(buff, (w>>12) & 0xf );
            __add4bits(buff, (w>>8)  & 0xf );
            __add4bits(buff, (w>>4)  & 0xf );
            __add4bits(buff,  w      & 0xf );
        }

        const char* ip_data<v6>:: hr() const throw()
        {
            const uint8_t *byte     = &addr[0]+16;
            char          *org      = &hrb.ch[0];
            char          *buff     = org;

            hrb.clear();

            for(size_t i=0;i<8;++i)
            {
                const uint16_t hi = *(--byte);
                const uint16_t lo = *(--byte);
                __add_hexa(buff, (hi<<8)|lo );
                if(i<7) *(buff++) = ':';
            }

            // compress
            size_t j = buff - org;
            buff     = org;
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
            
            return hrb.ch;
        }
        
    }
}
