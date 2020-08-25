
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
        
        typedef memory::tight::exp2_field<cplx_t> complexes;
        
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
                cplx_t   *L = *cplx; //mgr.acquire_field<cplx_t>(cplx_count,cplx_bytes,cplx_shift);
                
                
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
        
        static const uint16_t sq16[256] =
        {
            0x0000, 0x0001, 0x0004, 0x0009, 0x0010, 0x0019, 0x0024, 0x0031,
            0x0040, 0x0051, 0x0064, 0x0079, 0x0090, 0x00a9, 0x00c4, 0x00e1,
            0x0100, 0x0121, 0x0144, 0x0169, 0x0190, 0x01b9, 0x01e4, 0x0211,
            0x0240, 0x0271, 0x02a4, 0x02d9, 0x0310, 0x0349, 0x0384, 0x03c1,
            0x0400, 0x0441, 0x0484, 0x04c9, 0x0510, 0x0559, 0x05a4, 0x05f1,
            0x0640, 0x0691, 0x06e4, 0x0739, 0x0790, 0x07e9, 0x0844, 0x08a1,
            0x0900, 0x0961, 0x09c4, 0x0a29, 0x0a90, 0x0af9, 0x0b64, 0x0bd1,
            0x0c40, 0x0cb1, 0x0d24, 0x0d99, 0x0e10, 0x0e89, 0x0f04, 0x0f81,
            0x1000, 0x1081, 0x1104, 0x1189, 0x1210, 0x1299, 0x1324, 0x13b1,
            0x1440, 0x14d1, 0x1564, 0x15f9, 0x1690, 0x1729, 0x17c4, 0x1861,
            0x1900, 0x19a1, 0x1a44, 0x1ae9, 0x1b90, 0x1c39, 0x1ce4, 0x1d91,
            0x1e40, 0x1ef1, 0x1fa4, 0x2059, 0x2110, 0x21c9, 0x2284, 0x2341,
            0x2400, 0x24c1, 0x2584, 0x2649, 0x2710, 0x27d9, 0x28a4, 0x2971,
            0x2a40, 0x2b11, 0x2be4, 0x2cb9, 0x2d90, 0x2e69, 0x2f44, 0x3021,
            0x3100, 0x31e1, 0x32c4, 0x33a9, 0x3490, 0x3579, 0x3664, 0x3751,
            0x3840, 0x3931, 0x3a24, 0x3b19, 0x3c10, 0x3d09, 0x3e04, 0x3f01,
            0x4000, 0x4101, 0x4204, 0x4309, 0x4410, 0x4519, 0x4624, 0x4731,
            0x4840, 0x4951, 0x4a64, 0x4b79, 0x4c90, 0x4da9, 0x4ec4, 0x4fe1,
            0x5100, 0x5221, 0x5344, 0x5469, 0x5590, 0x56b9, 0x57e4, 0x5911,
            0x5a40, 0x5b71, 0x5ca4, 0x5dd9, 0x5f10, 0x6049, 0x6184, 0x62c1,
            0x6400, 0x6541, 0x6684, 0x67c9, 0x6910, 0x6a59, 0x6ba4, 0x6cf1,
            0x6e40, 0x6f91, 0x70e4, 0x7239, 0x7390, 0x74e9, 0x7644, 0x77a1,
            0x7900, 0x7a61, 0x7bc4, 0x7d29, 0x7e90, 0x7ff9, 0x8164, 0x82d1,
            0x8440, 0x85b1, 0x8724, 0x8899, 0x8a10, 0x8b89, 0x8d04, 0x8e81,
            0x9000, 0x9181, 0x9304, 0x9489, 0x9610, 0x9799, 0x9924, 0x9ab1,
            0x9c40, 0x9dd1, 0x9f64, 0xa0f9, 0xa290, 0xa429, 0xa5c4, 0xa761,
            0xa900, 0xaaa1, 0xac44, 0xade9, 0xaf90, 0xb139, 0xb2e4, 0xb491,
            0xb640, 0xb7f1, 0xb9a4, 0xbb59, 0xbd10, 0xbec9, 0xc084, 0xc241,
            0xc400, 0xc5c1, 0xc784, 0xc949, 0xcb10, 0xccd9, 0xcea4, 0xd071,
            0xd240, 0xd411, 0xd5e4, 0xd7b9, 0xd990, 0xdb69, 0xdd44, 0xdf21,
            0xe100, 0xe2e1, 0xe4c4, 0xe6a9, 0xe890, 0xea79, 0xec64, 0xee51,
            0xf040, 0xf231, 0xf424, 0xf619, 0xf810, 0xfa09, 0xfc04, 0xfe01
        };
        
        natural natural::square_of(const natural &x)
        {
            switch(x.bytes)
            {
                case 0: return natural();
                case 1: assert(x<256); return sq16[ x.word[0] ];
                default: break;
            }
            assert(x.bytes>0);
            assert(x>=256);
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
        }
        
    }
    
}

