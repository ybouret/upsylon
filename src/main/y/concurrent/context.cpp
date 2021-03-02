#include "y/concurrent/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/ints-utils.hpp"
#include "y/os/error.hpp"
#include "y/os/real-time-clock.hpp"
#include <cstring>
#include <cstdio>

namespace upsylon
{

    namespace concurrent
    {

        context:: ~context() throw()
        {
            _bzset(size);
            _bzset(rank);
            _bzset(indx);
            memset( (char *)label, 0, sizeof(label) );
        }

        context:: context() throw() :
        size(1),
        rank(0),
        indx(1),
        setw(core::count_digits::base10(size)),
        label()
        {
            fmt();
        }


        context:: context(const size_t sz,
                          const size_t rk) throw() :
        size(sz),
        rank(rk),
        indx(rank+1),
        setw(core::count_digits::base10(size)),
        label()
        {
            assert(size>=1);
            assert(rank<size);
            fmt();
        }
        
        
        context:: context(const context &other) throw() :
        size(other.size),
        rank(other.rank),
        indx(other.indx),
        setw(other.setw),
        label()
        {
            memcpy( (void*)label,other.label,sizeof(label));
        }

        void context:: fmt() throw()
        {
            static const size_t max_width = sizeof(label);

            char *str = (char *)label;
            memset(str,0,sizeof(label));
            const unsigned width  = (1+setw) * 2; // digits + '.' + digits + 0
            if(width>max_width)
            {
                fatal_error("concurrent::context(size is too big)");
            }

            char fmtstr[32] = { 0 };
            sprintf(fmtstr,"%%0%uu.%%0%uu",unsigned(setw),unsigned(setw));
            snprintf(str,max_width-1,fmtstr,unsigned(size),unsigned(rank));

        }

        uint64_t context:: ticks(lockable &sync) const throw()
        {
            Y_LOCK(sync);
            return real_time_clock::ticks();
        }


    }

}
