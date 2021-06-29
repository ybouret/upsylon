#include "y/gfx/color/ramp.hpp"
#include "y/gfx/pixel/interpolation.hpp"

namespace upsylon
{
    namespace graphic
    {

        rgba color_ramp:: build(const float u, const accessible<rgba> &arr) throw()
        {
            switch(arr.size())
            {
                case 0: return rgba(0,0,0);
                case 1: return arr[1];
                default:
                    break;
            }
            return interp::linear(u,arr);
        }

        rgba color_ramp:: build(const float u, const accessible<float> &pos, const accessible<rgba> &arr) throw()
        {
            assert(pos.size()==arr.size());
            const size_t num = arr.size();
            switch(num)
            {
                case 0: return rgba(0,0,0);
                case 1: return arr[1];
                default:
                    break;
            }
            size_t j=0;
            return interp::linear<rgba,const accessible<float>,const accessible<rgba> >(u,pos,arr,num,j);
        }

    }
}

