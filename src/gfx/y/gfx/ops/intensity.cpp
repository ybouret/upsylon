#include "y/gfx/ops/intensity.hpp"

namespace upsylon
{
    namespace graphic
    {
        intensity:: intensity() throw(): vmin(0), vmax(0), scal(0)
        {

        }

        intensity:: ~intensity() throw()
        {
        }

        void intensity:: load(const broker &apply) throw()
        {
            const float *arr = apply.caches.minmax<float>();
            aliasing::_(vmin) = arr[0];
            aliasing::_(vmax) = arr[1];
            if(vmax>vmin)
            {
                aliasing::_(scal) = 1.0f/(vmax-vmin);
            }
            else
            {
                aliasing::_(scal) = 0.0f;
            }
        }

        void intensity:: format(broker &apply) const
        {
            apply.caches.make<float>(4);
            for(size_t i=0;i<apply.size();++i)
            {
                apply.caches[i]->get<float>(1) = vmin;
                apply.caches[i]->get<float>(2) = vmax;
                apply.caches[i]->get<float>(3) = scal;
            }
        }

    }

}

