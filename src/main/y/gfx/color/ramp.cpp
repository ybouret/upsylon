#include "y/gfx/color/ramp.hpp"
#include "y/gfx/pixel/interpolation.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace graphic
    {
        color_ramp:: ~color_ramp() throw()
        {
        }

        color_ramp:: color_ramp() throw()
        {

        }



        rgba color_ramp:: get(const float u) const throw()
        {
            assert(u>=0.0f);
            assert(u<=1.0f);
            const size_t sz = dimensions();
            const rgba  *rp = repository();

            switch(sz)
            {
                case 0: return rgba(0,0,0);
                case 1: return rp[0];
                default:
                    break;
            }
            const float *sp = stop_point();
            if(sp)
            {
                size_t j = 0;
                return interp::linear<
                rgba,
                const float * const,
                const rgba  * const>(u,--sp,--rp,sz,j);
            }
            else
            {
                return interp::linear(u,rp,sz);
            }
        }

    }
}

#if 0
namespace upsylon
{
    namespace graphic
    {

        color_ramp:: mapping:: ~mapping() throw()
        {
        }

        color_ramp:: mapping:: mapping(const color_ramp &ramp_,
                                       const float       vmin_,
                                       const float       vmax_) throw() :
        vmin( min_of(vmin_,vmax_) ),
        vmax( max_of(vmin_,vmax_) ),
        scal( vmin<vmax ? 1.0f/(vmax-vmin) : 0.0f ),
        ramp( ramp_ )
        {

        }


        unit_t color_ramp:: mapping:: depth() const throw()
        {
            return sizeof(float);
        }

        rgba color_ramp:: mapping:: operator()(const void *addr) const throw()
        {
            assert(addr);
            const float value = *static_cast<const float *>(addr);
            if(value<=vmin)
            {
                return ramp.get(0.0f);
            }
            else
            {
                if(value>=vmax)
                {
                    return ramp.get(1.0f);
                }
                else
                {
                    return ramp.get( scal * (value-vmin) );
                }
            }

        }
    }
}
#endif
