#include "y/gfx/color/ramp.hpp"
#include "y/gfx/pixel/interpolation.hpp"

namespace upsylon
{
    namespace graphic
    {

        rgba color_mix:: build(const float u, const repository_type &arr) throw()
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

        rgba color_mix:: build(const float u, const accessible<float> &pos, const repository_type &arr) throw()
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

        rgba color_mix:: get(const float u) const throw()
        {
            return build(u,repository());
        }

        unit_t color_mix:: depth() const throw()
        {
            return sizeof(float);
        }



    }
}


namespace upsylon
{
    namespace graphic
    {

        color_ramp:: color_ramp() throw() : color_mix() {}

        color_ramp:: ~color_ramp() throw()
        {}

        rgba color_ramp:: operator()(const void *addr) const throw()
        {
            assert(NULL!=addr);
            return get( *(const float *)addr );
        }



    }

}

