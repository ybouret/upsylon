#include "y/gfx/color/ramp.hpp"
#include "y/gfx/pixel/interpolation.hpp"

namespace upsylon
{
    namespace graphic
    {

        color_ramp_:: ~color_ramp_() throw()
        {
        }

        color_ramp_:: color_ramp_() throw()
        {
        }



        rgba color_ramp_:: get(const float u) const throw()
        {
            const color_repository &arr = repository();
            switch(arr.size())
            {
                case 0: return rgba(0,0,0);
                case 1: return arr[1];
                default:
                    break;
            }
            return interp::linear(u,arr);
        }

    }

}




#if 0
rgba color_ramp:: build(const float u, const repository_type &arr) throw()
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
#endif

#if 0
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
#endif




namespace upsylon
{
    namespace graphic
    {

        color_ramp:: ~color_ramp() throw()
        {
        }

        color_ramp:: color_ramp(color_ramp_ *r) throw() :
        arc_ptr<color_ramp_>(r),
        type_to_rgba()
        {
        }

        color_ramp:: color_ramp(const color_ramp &r) throw() :
        arc_ptr<color_ramp_>(r),
        type_to_rgba()
        {
        }

        color_ramp & color_ramp:: operator=(const color_ramp &r) throw()
        {
            arc_ptr<color_ramp_> &self = *this;
            self = r;
            return *this;
        }



        unit_t color_ramp:: depth() const throw()
        {
            return sizeof(float);
        }

        rgba color_ramp:: operator()(const void *addr) const throw()
        {
            assert(NULL!=addr);
            return (**this).get( *(const float *)addr );
        }

    }
}





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

