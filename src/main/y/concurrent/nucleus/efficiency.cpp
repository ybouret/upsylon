#include "y/concurrent/nucleus/thread.hpp"

#include <cmath>

namespace upsylon
{

    namespace concurrent
    {
        namespace nucleus
        {
            double thread:: efficiency(const double speed_up,
                                       const size_t size) throw()
            {

                if(size<=1)
                {
                    return floor(10000.0*speed_up+0.5)/100.0;
                }
                else
                {
                    return floor(10000.0*(speed_up-1.0)/(size-1)+0.5)/100.0;
                }
            }
        }

    }
}

