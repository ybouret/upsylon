
#include "y/gfx/color/map.hpp"
#include "y/type/utils.hpp"


namespace upsylon
{
    namespace graphic
    {
        color_map:: ~color_map() throw()
        {
        }

        color_map:: color_map(const color_ramp &ramp_,
                              const float       vmin_,
                              const float       vmax_) throw() :
        ramp(ramp_),
        vmin(min_of(vmin_,vmax_)),
        vmax(max_of(vmin_,vmax_)),
        scal(vmax-vmin)
        {
            if(scal>0.0f) aliasing::_(scal) = 1.0f/scal;
        }

        float color_map:: v2u(const float v) const throw()
        {
            if(v<=vmin)
            {
                return 0.0f;
            }
            else
            {
                if(v>=vmax)
                {
                    return 1.0f;
                }
                else
                {
                    return clamp(0.0f,scal*(v-vmin),1.0f);
                }
            }
        }

        unit_t color_map :: depth() const throw() { return sizeof(float); }

        rgba   color_map :: operator()(const void *addr) const throw()
        {
            assert(addr);
            return ramp->get( v2u( *(const float *)addr) );
        }

    }
}
