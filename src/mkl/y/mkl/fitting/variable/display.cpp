
#include "y/mkl/fitting/variable/display.hpp"


namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            const char * const display_variables:: empty_separator = "";
            const char * const display_variables:: equal_separator = " = ";

            string display_variables:: real2text( const double u)
            {
                return vformat("%.15g",u);
            }

        }
    }
}

