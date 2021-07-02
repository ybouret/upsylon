#include "y/gfx/color/ramp/tableau.hpp"


namespace upsylon
{
    namespace graphic
    {
        fixed_color_ramp:: ~fixed_color_ramp() throw() {}

        fixed_color_ramp:: fixed_color_ramp(const rgba *data, const size_t size) throw():
        repo( (rgba *)data,size)
        {}


        const color_repository & fixed_color_ramp:: repository() const throw()
        {
            return repo;
        }
    }

}


