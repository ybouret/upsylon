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






