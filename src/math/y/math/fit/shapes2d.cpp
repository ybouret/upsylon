#include "y/math/fit/shapes2d.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            Shape2D:: ~Shape2D() throw()
            {
            }

            Shape2D:: Shape2D( const size_t num_arrays ) :
            Arrays(num_arrays)
            {
            }

        }
        
    }

}

