
#include "y/mpl/natural.hpp"
#include "y/mpl/memory.hpp"
#include "y/fft/fft.hpp"

namespace upsylon
{

    namespace mpl
    {

        typedef double          real_t;
        typedef complex<real_t> cplx_t;

        //! assuming fft1 is zeroed, same size than fft2, aka 2*n
        static inline
        void fft_mul(const uint8_t *data1,
                     const size_t   size1,
                     const uint8_t *data2,
                     const size_t   size2,
                     real_t        *fft1,
                     real_t        *fft2,
                     const size_t   n) throw()
        {
            assert( data1 != NULL );
            assert( data2 != NULL );
            assert( fft1  != NULL );
            assert( fft2  != NULL );

            assert( is_a_power_of_two(n) );
            assert(size1<=n);
            assert(size2<=n);

            //__________________________________________________________________
            //
            // packing real data in fft1
            //__________________________________________________________________

            // set real part
            for(size_t i=0;i<size1;++i)
            {
                fft1[(i<<1)] = data1[i];
            }
            // set imaginary part
            for(size_t i=0;i<size2;++i)
            {
                fft1[(i<<1)+1] = data2[i];
            }

            //__________________________________________________________________
            //
            // forward transform complex fft1
            //__________________________________________________________________
            cplx_t *L = (cplx_t *)fft1;
            cplx_t *R = (cplx_t *)fft2;
            --fft1;
            --fft2;
            fft<real_t>::forward(fft1,n);

            //__________________________________________________________________
            //
            // decompose fourier transforms
            //__________________________________________________________________
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

            for(size_t i=0;i<n;++i)
            {
                L[i] *= R[i];
            }

            fft<real_t>::reverse(fft1,n);

        }

        natural natural::__mul(const uint8_t *l, const size_t nl,
                               const uint8_t *r, const size_t nr)
        {

            if(nl>0&&nr>0)
            {
                static dispatcher &mgr = dispatcher::instance();

                const size_t np = nl+nr;             //-- product size
                natural      P(np, as_capacity );    //-- product value

                //--------------------------------------------------------------
                //-- compute common power of two
                //--------------------------------------------------------------
                const size_t nn = P.allocated; assert( is_a_power_of_two(nn) ); assert(nn>=np);


                size_t       n2     = nn<<1; assert(is_a_power_of_two(n2));
                size_t       wbytes = 0;
                size_t       wshift = 0;
                cplx_t      *L      = mgr.acquire_field<cplx_t>(n2,wbytes,wshift);
                cplx_t      *R      = L+nn;

                fft_mul(l,nl,r,nr,&L[0].re,&R[0].re,nn);

                const real_t fac   = 1.0/nn;
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = np - 1;
                for(size_t i=0;i<top;++i)
                {
                    const real_t q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                    const real_t r = carry - (256.0 * q);
                    prod[i]        = uint8_t(r);
                    carry          = q;
                }
                prod[top] = uint8_t(carry);
                P.bytes   = np;
                P.update();
                mgr.release_field(L,n2,wbytes,wshift);
                return P;
            }
            else
            {
                return natural();
            }
        }

        natural natural:: square_of( const natural &lhs )
        {
            const size_t nl = lhs.bytes;  
            if( nl > 0  )
            {
                static dispatcher &mgr = dispatcher::instance();

                const size_t np = nl << 1;            //-- product size
                natural      P( np, as_capacity );    //-- product value
                //--------------------------------------------------------------
                //-- compute power of two
                //--------------------------------------------------------------
                const size_t nn = P.allocated; assert( is_a_power_of_two(nn) ); assert(nn>=np);

                //--------------------------------------------------------------
                //- compute wokspace size and create it
                //--------------------------------------------------------------
                size_t         wbytes = 0;
                size_t         wshift = 0;
                size_t         nreq   = nn;
                cplx_t        *L      = mgr.acquire_field<cplx_t>(nreq,wbytes,wshift);
                {
                    const uint8_t *l = lhs.byte;
                    for(size_t i=0;i<nl;++i)
                    {
                        L[i].re = l[i];
                    }
                }

                //--------------------------------------------------------------
                //-- forward transform
                //--------------------------------------------------------------
                real_t *data = &L[0].re-1;
                fft<real_t>::forward( data,  nn  );

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                for(size_t i=0;i<nn;++i)
                {
                    L[i].__square();
                }
                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                fft<real_t>::reverse( data, nn );
                const real_t fac   = 1.0/nn;
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = np - 1;
                for(size_t i=0;i<top;++i)
                {
                    const real_t q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                    const real_t r = carry - (256.0 * q);
                    prod[i]        = uint8_t(r);
                    carry          = q;
                }
                assert(top<P.allocated);
                prod[top] = uint8_t(carry);
                P.bytes = np;
                P.update();
                mgr.release_field(L,nreq,wbytes,wshift);
                return P;
            }
            else
                return natural();
        }

    }
}

