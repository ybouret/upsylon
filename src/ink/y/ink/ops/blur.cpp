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
        weight(length+1)
        {
            float *w = *weight;
            w[0] = 1.0f;
            for(unit_t i=length;i>0;--i)
            {
                w[i] = expf( -(i*i)/scale );
            }
        }
        
        
    }
}
