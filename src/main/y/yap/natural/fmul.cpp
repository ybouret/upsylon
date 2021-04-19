#include "y/yap/natural.hpp"
#include "y/code/base2.hpp"
#include "y/fft/fft1.hpp"
#include "y/os/endian.hpp"
#include "y/type/complex.hpp"

namespace upsylon
{
    namespace yap
    {
        
        typedef double             real_t;
        typedef complex<real_t>    cplx_t;
        typedef natural::word_type word_t;

        namespace
        {
            template <
            const size_t LENGTH,
            const size_t OFFSET> struct transfer;

            template <const size_t OFFSET>
            struct transfer<1,OFFSET>
            {
                static inline size_t run(real_t        *fft1,
                                         const uint8_t *byte,
                                         size_t         i) throw()
                {
                    fft1[ (i<<1)+OFFSET ] = byte[0];
                    return ++i;
                }
            };

            template <const size_t OFFSET>
            struct transfer<2,OFFSET>
            {
                static inline size_t run(real_t        *fft1,
                                         const uint8_t *byte,
                                         const size_t   i) throw()
                {
                    fft1[ ( (i+0)<<1 )+OFFSET ] = byte[0];
                    fft1[ ( (i+1)<<1 )+OFFSET ] = byte[1];
                    return i+2;
                }
            };


            template <const size_t OFFSET>
            struct transfer<4,OFFSET>
            {
                static inline size_t run(real_t        *fft1,
                                         const uint8_t *byte,
                                         const size_t   i) throw()
                {
                    fft1[ ( (i+0)<<1 )+OFFSET ] = byte[0];
                    fft1[ ( (i+1)<<1 )+OFFSET ] = byte[1];
                    fft1[ ( (i+2)<<1 )+OFFSET ] = byte[2];
                    fft1[ ( (i+3)<<1 )+OFFSET ] = byte[3];
                    return i+4;
                }
            };

        }


        static inline
        void encode_re(real_t       *fft1,
                       const word_t *lhs,
                       const size_t  lnw) throw()
        {
            size_t i=0;
            for(size_t j=0;j<lnw;++j)
            {

#if 1

#    if Y_BYTE_ORDER == Y_LIT_ENDIAN
                const word_t w = lhs[j];
#    else
                const word_t w = swap_le(lhs[j]);
#    endif
                i = transfer<sizeof(word_t),0>::run(fft1,(const uint8_t *)&w,i);
#else
                word_t w = lhs[j];
                for(size_t k=0;k<sizeof(word_t);++k,++i)
                {
                    fft1[ (i<<1) ] = uint8_t(w);
                    w >>= 8;
                }
#endif

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

#if 1

#    if Y_BYTE_ORDER == Y_LIT_ENDIAN
                const word_t w = rhs[j];
#    else
                const word_t w = swap_le(rhs[j]);
#    endif
                i = transfer<sizeof(word_t),1>::run(fft1,(const uint8_t *)&w,i);
#else
                word_t w = rhs[j];
                for(size_t k=0;k<sizeof(word_t);++k,++i)
                {
                    fft1[ (i<<1)+1 ] = uint8_t(w);
                    w >>= 8;
                }
#endif
            }
        }
        
        
        static inline
        void decode_data(real_t      *fft1,
                         real_t      *fft2,
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

#if 0
                const real_t rep=(0.5)*(fft1[j]   + fft1[nn2mj]);
                const real_t rem=(0.5)*(fft1[j]   - fft1[nn2mj]);
                const real_t aip=(0.5)*(fft1[jp1] + fft1[nn3mj]);
                const real_t aim=(0.5)*(fft1[jp1] - fft1[nn3mj]);
#endif

                const real_t A  = fft1[j];
                const real_t B  = fft1[nn2mj];
                const real_t C  = fft1[jp1];
                const real_t D  = fft1[nn3mj];

                const real_t rep=(0.5)*(A+B);
                const real_t rem=(0.5)*(A-B);
                const real_t aip=(0.5)*(C+D);
                const real_t aim=(0.5)*(C-D);


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
                      const size_t  size,
                      const cplx_t *L,
                      const size_t  nn) throw()
        {
            const real_t fac   = 1.0/nn;
            real_t       carry = 0.0;
            const size_t top   = size - 1;
            for(size_t i=0;i<top;++i)
            {
                const real_t q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                const real_t r = carry - (256.0 * q);
                prod[i]        = uint8_t(r);
                carry          = q;
            }
            prod[top] = uint8_t(carry);

#    if Y_BYTE_ORDER != Y_LIT_ENDIAN
            const size_t  nw = natural::words_for(size);
            word_t       *w  = (word_t *)prod;
            for(size_t i=0;i<nw;++i)
            {
                w[i] = swap_le(w[i]);
            }
#endif
        }
        
        typedef memory::tight::exp2_field<cplx_t> complexes;
        
        natural natural:: fmul(const word_type *lhs, const size_t lnw,
                               const word_type *rhs, const size_t rnw)
        {
            assert(lhs);
            assert(rhs);
            assert(lnw>0);
            assert(rnw>0);
            
            
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
            cplx_t   *L = *cplx;
            
            
            {
                //----------------------------------------------------------
                // encode data
                //----------------------------------------------------------
                real_t   *fft1 = &L[0].re;
                encode_re(fft1,lhs,lnw);
                encode_im(fft1,rhs,rnw);
                
                //----------------------------------------------------------
                // forward transform
                //----------------------------------------------------------
                --fft1;
                FFT1::Forward(fft1,nn);
                
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
                // reverse transform
                //----------------------------------------------------------
                FFT1::Reverse(fft1,nn);
            }
            
            //--------------------------------------------------------------
            // finalizing product
            //--------------------------------------------------------------
            finalize((uint8_t *)(p.word),p_bytes,L,nn);
            
            
            p.bytes=p_bytes;
            p.update();
            return p;
            
        }
        
        
        
        natural natural::fsquare(const natural &x)
        {
            assert(x>0);
            
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
                FFT1::Forward(fft1,nn);
                for(size_t i=0;i<nn;++i)
                {
                    L[i].__square();
                }
                FFT1::Reverse(fft1,nn);
            }
            
            //--------------------------------------------------------------
            // finalizing product
            //--------------------------------------------------------------
            finalize((uint8_t *)(p.word), p_bytes, L,nn);
            
            p.bytes=p_bytes;
            p.update();
            return p;
        }
        
        
    }
    
}

