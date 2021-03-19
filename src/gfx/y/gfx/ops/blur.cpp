
#include "y/gfx/ops/blur.hpp"
#include "y/exceptions.hpp"
#include <cstdlib>

namespace upsylon
{
    namespace graphic
    {

        const float blur::expand = 2.355f;

        unit_t blur:: delta_for(const float sig) throw()
        {
            assert(sig>0);
            return unit_t(1.0f+floorf( expand * sig + 0.5f ));
        }

        static float check_sigma(const float sig)
        {
            if(sig<=0) throw exception("invalid blur sigma=%g",sig);
            return sig;
        }
        
        
        blur:: blur(const float sig) :
        delta( delta_for(check_sigma(sig) ) ),
        weight( 1+twice(square_of(delta))   ),
        factor(0),
        counts(0),
        length(1+twice(delta)),
        sigma(sig)
        {
            
            const float coeff = 1.0f/twice(square_of(sigma));
            {
                for(unit_t y=-delta;y<=delta;++y)
                {
                    const unit_t y2 = y*y;
                    for(unit_t x=-delta;x<=delta;++x)
                    {
                        const unit_t x2=x*x;
                        const unit_t r2=x2+y2;
                        weight_t &w = aliasing::_(weight[r2]);
                        w.value = expf( -float(r2)*coeff );
                        w.count++;
                    }
                }
            }
            float  sum = 0.0f;
            size_t num = 0;
            for(size_t i=weight.size;i>0;)
            {
                const weight_t &w = weight[--i];
                const count_t   n = w.count;
                
                if(n>0)
                {
                    num += n;
                    sum += n * w.value;
                    std::cerr << "r^2=" << i << "x" << w.count << " => " << w.value << std::endl;
                }
            }
            aliasing::_(factor) = 1.0f / sum;
            aliasing::_(counts) = num;
            std::cerr << "factor=" << factor << " | counts=" << counts << std::endl;
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

