
#include "y/gfx/ops/blur.hpp"
#include "y/exceptions.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon
{
    namespace graphic
    {

        const float blur::expand = 2.355f;

        unit_t blur:: r_max_for(const float sig) throw()
        {
            assert(sig>0);
            return unit_t(1.0f+floorf( expand * sig + 0.5f ));
        }

        static float check_sigma(const float sig)
        {

            return sig;
        }

        blur:: blur(const float sig) :
        r_max( r_max_for(check_sigma(sig) ) ),
        weight(1+r_max,1+r_max),
        factor(0),
        sigma( sig )
        {
            const float coeff = 1.0f/twice(square_of(sigma));
            vector<float,memory::dyadic> w( square_of(1+2*r_max), 0.0f);
            {
                size_t k = 0;
                for(unit_t y=-r_max;y<=r_max;++y)
                {
                    const float Y2(y*y);
                    for(unit_t x=-r_max;x<=r_max;++x)
                    {
                        const float X2(x*x);
                        const float EW = expf( -(X2+Y2)*coeff );
                        w[++k] = EW;
                        if(x>=0&&y>=0)
                        {
                            aliasing::_(weight(y)(x)) = EW;
                        }
                    }
                }
                assert(k==w.size());
            }
            aliasing::_(factor) = 1.0f/sorted_sum(w);
        }

        blur:: ~blur() throw()
        {
        }




        

    }

}

