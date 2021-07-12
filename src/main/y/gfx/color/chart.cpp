
#include "y/gfx/color/chart.hpp"


namespace upsylon
{

    namespace graphic
    {

        color_chart:: color_chart(const color_ramp &ramp_) :
        type_to_rgba(),
        ramp(ramp_)
        {
        }

        color_chart:: ~color_chart() throw()
        {
        }

        unit_t color_chart:: depth() const throw()
        {
            return sizeof(uint8_t);
        }

        rgba color_chart:: operator()(const void *addr) const throw()
        {
            assert(addr);
            return ramp->get( crux::convert::unit_float[ *(const uint8_t *)addr ] );
        }



    }

}

