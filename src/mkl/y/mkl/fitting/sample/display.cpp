

 #include "y/mkl/fitting/sample/display.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            string display_sample:: size2text(const size_t n)
            {
                return vformat("%lu", (unsigned long)n );
            }
        }
    }
}

