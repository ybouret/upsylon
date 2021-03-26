#include "y/gfx/color/ramp.hpp"
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

        rgba color_ramp:: get(const float u) const throw()
        {
            const size_t sz = dimensions();
            const rgba  *rp = repository();

            switch(sz)
            {
                case 0: return rgba(0,0,0);
                case 1: return rp[0];
                default:
                    break;
            }

            const float  smax = float(sz-1);
            size_t       jlo  = 0;
            size_t       jup  = 0;
            const float  fpos = u * smax;         // map 0:1 to 0..smax
            const float  ipos = floor(fpos+0.5f); // nearest integer
            if(ipos<=fpos)
            {
                jlo = size_t(max_of(ipos,0.0f));
                jup = jlo+1; assert(jup<=sz);
            }
            else
            {
                jup = size_t(min_of(ipos,smax)); assert(jup>0);
                jlo = jup-1;
            }

            const float w0 = float(jup) - fpos;
            const float w1 = fpos - float(jlo);
            const rgba  C0 = rp[jlo];
            const rgba  C1 = rp[jup];


            const rgb res = rgb(mix_bytes(C0.r,w0,C1.r,w1),
                                mix_bytes(C0.g,w0,C1.g,w1),
                                mix_bytes(C0.b,w0,C1.b,w1));

            //std::cerr << w0 << '*' << C0 << '+' << w1 << '*' << C1 << " => " << res << std::endl;
            return res;
        }

    }
}

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
