
#include "y/mpl/natural.hpp"
#include <cstring>

namespace upsylon {
    
    namespace mpl {
        
        void    natural:: __smul(uint8_t       *b,
                                 size_t        &m,
                                 const uint8_t *a,
                                 const size_t   n,
                                 const uint8_t  x) throw()
        {
            assert(b);
            assert(a);
            assert(!(n>0&&a[n-1]<=0));
            assert(0==m);
            if(n<=0)
            {
                memset(b,0,n+1); return;
            }
            else
            {
                switch(x)
                {
                    case 0: memset(b,0,n+1);           return;
                    case 1: memcpy(b,a,m=n); b[n] = 0; return;
                    default:
                        break;
                }
                unsigned carry = a[0] * x;
                b[0] = uint8_t( carry & 0xff );
                carry >>= 8;
                for(size_t i=1;i<n;++i)
                {
                    carry += a[i] * x;
                    b[i]   = uint8_t( carry & 0xff );
                    carry >>= 8;
                }
                switch( (b[n]=carry) )
                {
                    case  0: m=n;   break;
                    default: m=n+1; break;
                }
            }
            
        }
        
        
        natural natural:: short_mul(const natural &lhs, const uint8_t x)
        {
            const uint8_t *a = lhs.byte;
            const size_t   n = lhs.bytes;
            natural        b(n+1,as_capacity);
            
            __smul(b.byte,b.bytes,a,n,x);
            return b;
        }
        
    }
    
    
}
