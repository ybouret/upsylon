
#include "y/yap/natural.hpp"
#include "y/code/base2.hpp"
#include "y/fft/fft.hpp"
#include "y/os/endian.hpp"
namespace upsylon
{
    namespace yap
    {
        
        typedef double             real_t;
        typedef complex<real_t>    cplx_t;
        typedef natural::word_type word_t;
        
        
        static inline
        void encode_re(real_t       *fft1,
                       const word_t *lhs,
                       const size_t  lnw) throw()
        {
            size_t i=0;
            for(size_t j=0;j<lnw;++j)
            {
                word_t w = lhs[j];
                for(size_t k=0;k<sizeof(word_t);++k,++i)
                {
                    fft1[ (i<<1) ] = uint8_t(w);
                    w >>= 8;
                }
            }
        }
        
        
        static inline
        void encode_im(real_t       *fft1,
                       const word_t *rhs,
                       const size_t  rnw) throw()
        {
            size_t i=0;
            for(size_t j=0;j<rnw;++j)
            {
                word_t w = rhs[j];
                for(size_t k=0;k<sizeof(word_t);++k,++i)
                {
                    fft1[ (i<<1)+1 ] = uint8_t(w);
                    w >>= 8;
                }
            }
        }
        
        
        static inline
        void decode_data(real_t *fft1,
                         real_t *fft2,
                         const size_t n) throw()
        {
            const size_t np1 = n+1;
            const size_t nn2 = np1<<1;
            const size_t nn3 = nn2+1;
            
            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=0;
            for(size_t j=3;j<=np1;j+=2)
            {
                const size_t jp1   = j+1;
                const size_t nn2mj = nn2 - j;
                const size_t nn3mj = nn3 - j;
                const real_t rep=(0.5)*(fft1[j]   + fft1[nn2mj]);
                const real_t rem=(0.5)*(fft1[j]   - fft1[nn2mj]);
                const real_t aip=(0.5)*(fft1[jp1] + fft1[nn3mj]);
                const real_t aim=(0.5)*(fft1[jp1] - fft1[nn3mj]);
                fft1[j]     =  rep;
                fft1[jp1]   =  aim;
                fft1[nn2mj] =  rep;
                fft1[nn3mj] = -aim;
                fft2[j]     =  aip;
                fft2[jp1]   = -rem;
                fft2[nn2mj] =  aip;
                fft2[nn3mj] =  rem;
            }
        }
        
        
        static inline
        void finalize(uint8_t      *prod,
                      const size_t  p_bytes,
                      const cplx_t *L,
                      const size_t  nn) throw()
        {
            const real_t fac   = 1.0/nn;
            real_t       carry = 0.0;
            const size_t top   = p_bytes - 1;
            for(size_t i=0;i<top;++i)
            {
                const real_t q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                const real_t r = carry - (256.0 * q);
                prod[i]        = uint8_t(r);
                carry          = q;
            }
            prod[top] = uint8_t(carry);
            
            const size_t  nw = natural::words_for(p_bytes);
            word_t       *w  = (word_t *)prod;
            for(size_t i=0;i<nw;++i)
            {
                w[i] = swap_le(w[i]);
            }
        }
        
        typedef memory::tight::quarry_field<cplx_t> complexes;
        
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
                complexes cplx(mgr,nn<<1);
                cplx_t *L = *cplx; //mgr.acquire_field<cplx_t>(cplx_count,cplx_bytes,cplx_shift);
                
                
                {
                    //----------------------------------------------------------
                    // encode data
                    //----------------------------------------------------------
                    real_t *fft1 = &L[0].re;
                    encode_re(fft1,lhs,lnw);
                    encode_im(fft1,rhs,rnw);
                    
                    //----------------------------------------------------------
                    // forward transform
                    //----------------------------------------------------------
                    --fft1;
                    fft<real_t>::forward(fft1,nn);
                    
                    //----------------------------------------------------------
                    // decode data
                    //----------------------------------------------------------
                    cplx_t *R          = L+nn;
                    decode_data(fft1,&R[0].re-1,nn);
                    
                    //----------------------------------------------------------
                    // convolution
                    //----------------------------------------------------------
                    for(size_t i=0;i<nn;++i)
                    {
                        L[i] *= R[i];
                    }
                    
                    //----------------------------------------------------------
                    //reverse transform
                    //----------------------------------------------------------
                    fft<real_t>::reverse(fft1,nn);
                    
                }
                
                //--------------------------------------------------------------
                // finalizing product
                //--------------------------------------------------------------
                finalize((uint8_t *)(p.word), p_bytes, L,nn);
                
                
                p.bytes=p_bytes;
                p.update();
                return p;
                
            }
            else
            {
                return natural();
            }
        }

        natural natural::square_of(const natural &x)
        {
            if(x.bytes)
            {
                static memory_allocator &mgr = instance();

                //--------------------------------------------------------------
                // prepare product and intenal memory
                //--------------------------------------------------------------
                const size_t p_bytes = x.bytes << 1; natural      p(p_bytes,as_capacity);
                const size_t nn      = p.width;      complexes    cplx(mgr,nn);
                cplx_t *     L       = *cplx;

                //--------------------------------------------------------------
                // auto-correlation
                //--------------------------------------------------------------
                {
                    real_t      *fft1 = &L[0].re;
                    encode_re(fft1,x.word,x.words);
                    --fft1;
                    fft<real_t>::forward(fft1,nn);
                    for(size_t i=0;i<nn;++i)
                    {
                        L[i].__square();
                    }
                    fft<real_t>::reverse(fft1,nn);
                }

                //--------------------------------------------------------------
                // finalizing product
                //--------------------------------------------------------------
                finalize((uint8_t *)(p.word), p_bytes, L,nn);


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
