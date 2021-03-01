#include "y/concurrent/nucleus/thread.hpp"

#include <cmath>

namespace upsylon
{

    namespace concurrent
    {
        namespace nucleus
        {
            static inline double percent_with_2digits(const double ratio) throw()
            {
                return floor(10000.0 * ratio + 0.5)/100.0;
            }

            double thread:: efficiency(const double speed_up,
                                       const size_t num_cpus) throw()
            {

                if(num_cpus<=1)
                {
                    return percent_with_2digits(speed_up);
                }
                else
                {
                    return percent_with_2digits(speed_up/num_cpus);
                }
            }
        }

    }
}

