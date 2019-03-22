
#include "y/concurrent/layout.hpp"
#include "y/type/utils.hpp"
#include "y/os/hw.hpp"
#include "y/string/env.hpp"
#include <iostream>
#include <cstdlib>

namespace upsylon
{
    namespace concurrent
    {
        layout:: ~layout() throw()
        {
        }

        layout:: layout(const size_t num_procs,
                        const size_t num_cores,
                        const size_t starting) :
        procs( max_of<size_t>(1,num_procs)      ),
        cores( clamp<size_t>(1,num_cores,procs) ),
        shift( min_of<size_t>(starting,procs-1) ),
        width( min_of<size_t>( cores, procs-shift) )
        {
            //std::cerr << "layout: using #core=" << cores << "/" << procs << ", starting@" << shift << "=>width=" << width << std::endl;
        }

        layout:: layout(const layout &other) throw() :
        procs(other.procs),
        cores(other.cores),
        shift(other.shift),
        width(other.width)
        {
        }
        
        size_t layout:: core_index_of(const size_t thread_index) const throw()
        {
            return shift + (thread_index%width);
        }

        layout * layout:: create()
        {
            const size_t num_procs = hardware::nprocs();
            string       value;
            const string name="Y_THREADING";
            if( environment::get(value,name) )
            {
                char *s_cores = *value;
                char *s_shift = strchr(s_cores, ':' );
                if(s_shift)
                {
                    *(s_shift++) = 0;
                }
                else
                {
                    s_shift = 0;
                }
                const size_t num_cores = atol(s_cores);
                const size_t starting  = (s_shift) ? atol(s_shift) : 0;
                return new layout(num_procs,num_cores,starting);
            }
            else
            {
                return new layout(num_procs,num_procs,0);
            }
        }

    }
}
