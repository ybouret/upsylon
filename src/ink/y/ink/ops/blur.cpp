#include "y/ink/ops/blur.hpp"

namespace upsylon
{
    namespace Ink
    {
        Blur:: ~Blur() throw()
        {
        }

        Blur:: Blur( const float sig ) :
        sigma( fabsf(sig) ),
        scale( 2*sigma*sigma ),
        length( 1 + unit_t( floorf( sqrtf(scale*logf(256.0f)))) ),
        counts(1+length),
        normal(counts,counts),
        weight(normal.r_aux1)
        {
            assert(counts==unit_t(weight.size()));
            float *w = *weight;
            w[0] = 1.0f;
            for(unit_t i=length;i>0;--i)
            {
                w[i] = expf( -float(i*i)/scale );
            }
            
            for(unit_t ihi=0;ihi<=length;++ihi)
            {
                array<float> &nrm = normal[ihi+1];
                for(unit_t ilo=0;ilo<=length;++ilo)
                {
                    float sum  = 0;
                    for(unit_t i=-ilo;i<=ihi;++i)
                    {
                        sum += w[ abs_of(i) ];
                    }
                    nrm[ilo+1] = sum;
                }
            }

        }
        
        
    }
}
