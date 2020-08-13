
#include "y/memory/tight/wedge.hpp"
#include "y/os/run-time-log.hpp"

namespace upsylon
{
    namespace memory
    {

        namespace tight
        {
            void __wedge:: leak(const size_t class_size, const size_t count) throw()
            {
                rtl(run_time_warning) << "[tight::wedge<" << class_size << ">] #leak=" << count << std::endl;
            }
            
        }
    }
}
