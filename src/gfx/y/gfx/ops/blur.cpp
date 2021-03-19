
#include "y/gfx/ops/blur.hpp"
#include "y/exceptions.hpp"
#include <cstdlib>

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

        static inline int cmpflt(const void *lhs, const void *rhs) throw()
        {
            const float L = fabsf( *static_cast<const float *>(lhs) );
            const float R = fabsf( *static_cast<const float *>(rhs) );
            return L<R ? -1 : ( R<L ? 1 : 0);

        }


        float blur:: direct_sum() const throw()
        {
            float        sum = 0;
            const size_t n   = data.size;
            for(size_t i=0;i<n;++i)
            {
                sum += data[i];
            }
            return sum;
        }

        float blur:: direct_average() const throw()
        {
            return direct_sum() * factor;
        }

        float blur:: sorted_sum() const throw()
        {
            qsort(&data[0],data.size,sizeof(float),cmpflt);
            return direct_sum();
        }

        float blur:: sorted_average() const throw()
        {
            return sorted_sum() * factor;
        }


        blur:: blur(const float sig) :
        r_max( r_max_for(check_sigma(sig) ) ),
        weight(1+r_max,1+r_max),
        factor(0),
        sigma( sig ),
        data(square_of(1+2*r_max))
        {
            const float coeff = 1.0f/twice(square_of(sigma));
            {
                size_t k = 0;
                for(unit_t y=-r_max;y<=r_max;++y)
                {
                    const float Y2(y*y);
                    for(unit_t x=-r_max;x<=r_max;++x)
                    {
                        const float X2(x*x);
                        const float EW = expf( -(X2+Y2)*coeff );
                        data[k++] = EW;
                        if(x>=0&&y>=0)
                        {
                            aliasing::_(weight(y)(x)) = EW;
                        }
                    }
                }
                assert(k==data.size);
            }
            aliasing::_(factor) = 1.0f/sorted_sum();
        }

        blur:: ~blur() throw()
        {
        }



        void blur:: compute(pixmap<float> &target, broker &apply,const pixmap<float> &source) const
        {
            compute_for<float,float,1>(target,apply,source);
        }

        void blur:: compute(pixmap<rgb>   &target, broker &apply,const pixmap<rgb>   &source) const
        {
            compute_for<rgb,uint8_t,3>(target,apply,source);

        }

        void blur:: compute(pixmap<rgba>   &target, broker &apply,const pixmap<rgba>   &source) const
        {
            compute_for<rgba,uint8_t,3>(target,apply,source);
        }

        void blur:: compute(pixmap<uint8_t>   &target, broker &apply,const pixmap<uint8_t>   &source) const
        {
            compute_for<uint8_t,uint8_t,1>(target,apply,source);
        }

        

    }

}

