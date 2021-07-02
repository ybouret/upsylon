#include "y/gfx/color/ramp/cold-to-hot.hpp"
#include "y/gfx/color/named.hpp"

namespace upsylon
{
    namespace graphic
    {

        const rgba cold_to_hot:: data[] =
        {
            Y_BLUE,
            Y_CYAN,
            Y_GREEN,
            Y_YELLOW,
            Y_RED
        };

        const size_t cold_to_hot:: size = sizeof(data)/sizeof(data[0]);

    }

}


