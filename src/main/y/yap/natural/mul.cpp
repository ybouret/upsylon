
#include "y/yap/natural.hpp"
#include "y/code/base2.hpp"
#include "y/fft/fft.hpp"

namespace upsylon
{
    namespace yap
    {
        
        typedef double          real_t;
        typedef complex<real_t> cplx_t;
        
        
        natural natural:: mul(const word_type *lhs, const size_t lnw,
                              const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                static memory_allocator &mgr = instance();
                
                //--------------------------------------------------------------
                // initialize product memory
                //--------------------------------------------------------------
                const size_t p_words = lnw+rnw;              // required words for product
                const size_t p_bytes = p_words << word_exp2; // required bytes for product
                natural      p(p_bytes,as_capacity);         // product
                
                //--------------------------------------------------------------
                // get common power of two bytes
                //--------------------------------------------------------------
                const size_t nn = p.width; assert( is_a_power_of_two(nn) ); assert(nn>=p_bytes);
                //--------------------------------------------------------------
                // get memory to hold two arrays of complexes
                //--------------------------------------------------------------
                size_t  cplx_count = nn << 1;
                size_t  cplx_bytes = 0;
                size_t  cplx_shift = 0;
                cplx_t *L          = mgr.acquire_field<cplx_t>(cplx_count,cplx_bytes,cplx_shift);
                cplx_t *R          = L+nn;
                
                //--------------------------------------------------------------
                // packing data
                //--------------------------------------------------------------
                
                
                //--------------------------------------------------------------
                // finalizing product
                //--------------------------------------------------------------
                {
                    const real_t fac   = 1.0/nn;
                    real_t       carry = 0.0;
                    uint8_t     *prod  = (uint8_t*)(p.word);
                    const size_t top   = p_bytes - 1;
                    for(size_t i=0;i<top;++i)
                    {
                        const real_t q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                        const real_t r = carry - (256.0 * q);
                        prod[i]        = uint8_t(r);
                        carry          = q;
                    }
                    prod[top] = uint8_t(carry);
                }
                
                mgr.release_field(L,cplx_count,cplx_bytes,cplx_shift);
                p.bytes=p_bytes;
                p.update();
                return p;
                
            }
            else
            {
                return natural();
            }
        }
        
        
    }
    
}
