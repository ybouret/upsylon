#include "y/gfx/ops/intensity.hpp"

namespace upsylon
{
    namespace graphic
    {
        intensity:: intensity() throw(): vmin(0), vmax(0)
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
        }

        void intensity:: format(broker &apply) const
        {
            apply.caches.make<float>(2);
            apply.caches[0]->as<float>() = vmin;
            apply.caches[1]->as<float>() = vmax;
        }

    }

}

