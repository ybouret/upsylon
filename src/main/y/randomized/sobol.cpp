#include "y/randomized/sobol.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <cstring>

namespace upsylon
{
    namespace randomized
    {
        sobol:: sobol() throw() :
        in(0),
        fac( 1.0/(1L<<MAXBIT) ),
        ix(),
        iu()
        {
            reset();
        }

        const unsigned long sobol:: mdeg[MAXDIM+1] =
        {
            0,1,2,3,3,4,4
        };

        const unsigned long sobol:: ip[MAXDIM+1] =
        {
            0,0,1,1,2,1,4
        };

        const unsigned long sobol::iv0[MAXDIM*MAXBIT+1] =
        {
            0,1,1,1,1,1,1,3,1,3,3,1,1,5,7,7,3,3,5,15,11,5,15,13,9
        };

        void sobol:: reset() throw()
        {
            memcpy(iv,iv0,sizeof(iv));
            assert( iv[1] == 1 );
            in = 0;
            for(size_t k=1;k<=MAXDIM;++k) ix[k]=0;
            for(size_t j=1,k=0;j<=MAXBIT;j++,k+=MAXDIM) iu[j] = &iv[k];
            for(size_t k=1;k<=MAXDIM;k++)
            {
                for (size_t j=1;j<=mdeg[k];j++)
                {
                    iu[j][k] <<= (MAXBIT-j);
                }

                for (size_t j=mdeg[k]+1;j<=MAXBIT;j++)
                {
                    unsigned long ipp = ip[k];
                    unsigned long i   = iu[j-mdeg[k]][k];
                    i ^= (i >> mdeg[k]);
                    for(unsigned long l=mdeg[k]-1;l>=1;l--)
                    {
                        if (ipp & 1) i ^= iu[j-l][k];
                        ipp >>= 1;
                    }
                    iu[j][k]=i;
                }
            }
        }

        unsigned long sobol:: cycle()
        {
            unsigned long im=in++;
            {
                unsigned long j =1;
                for(;j<=MAXBIT;++j)
                {
                    if (!(im & 1)) break;
                    im >>= 1;
                }
                if (j > MAXBIT)  throw exception("MAXBIT too small in sobol");
                im=(j-1)*MAXDIM;
            }
            return im;
        }

        void sobol:: compute(size_t n, double *x )
        {
            assert(!(n>0&&x==0));
            const unsigned long im = cycle();
            {
                const unsigned long kmax = min_of<unsigned long>(n,MAXDIM);
                --x;
                for(unsigned long k=1;k<=kmax;++k)
                {
                    ix[k] ^= iv[im+k];
                    x[k]=ix[k]*fac;
                }
            }
        }

        void sobol:: compute( array<double> &x )
        {
            const unsigned long im = cycle();
            {
                const unsigned long kmax = min_of<unsigned long>(x.size(),MAXDIM);
                for(unsigned long k=1;k<=kmax;++k)
                {
                    ix[k] ^= iv[im+k];
                    x[k]=ix[k]*fac;
                }
            }
        }

        void sobol:: skip1(const size_t n)
        {
            const unsigned long im = cycle();
            {
                const unsigned long kmax = min_of<unsigned long>(n,MAXDIM);
                for(unsigned long k=1;k<=kmax;++k)
                {
                    ix[k] ^= iv[im+k];
                }
            }
        }

    }
}
