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

#if 0
        static inline uint8_t mix_bytes(const uint8_t b0, const float w0,
                                        const uint8_t b1, const float w1) throw()
        {
            const float B0 = float(b0) * w0;
            const float B1 = float(b1) * w1;
            const float S  = B0+B1;
            if(S<=0)
            {
                return 0;
            }
            else
            {
                if(S>=255.0f)
                {
                    return 255;
                }
                else
                {
                    return uint8_t( floorf(S+0.5f) );
                }
            }
        }
#endif

        rgba color_ramp:: closest(const float u, const rgba *rp, const size_t sz) const throw()
        {
            assert(u>=0.0f);
            assert(u<=1.0f);
            assert(NULL!=rp);
            assert(sz>1);
            const float  delta = float(sz-1);
            const float  match = u * delta;
            const size_t index = size_t( floorf(match) + 0.5f);
            assert(index<sz);
            return rp[index];
        }

        rgba color_ramp:: blended(const float u, const rgba *rp, const size_t sz) const throw()
        {
            assert(u>=0.0f);
            assert(u<=1.0f);
            assert(NULL!=rp);
            assert(sz>1);
            return rp[0];
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

            return closest(u,rp,sz);
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
