#include "y/concurrent/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/ints-utils.hpp"
#include "y/os/error.hpp"
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
        label()
        {
            fmt();
        }


        context:: context(const size_t sz,
                          const size_t rk) throw() :
        size(sz),
        rank(rk),
        indx(rank+1),
        label()
        {
            assert(size>=1);
            assert(rank<size);
            fmt();
        }

        void context:: fmt() throw()
        {
            static const size_t max_width = sizeof(label);

            char *str = (char *)label;
            memset(str,0,sizeof(label));
            const unsigned digits = core::count_digits::base10(size);
            const unsigned width  = (1+digits) * 2; // digits + '.' + digits + 0
            if(width>max_width)
            {
                fatal_error("concurrent::context(size is too big)");
            }

            char fmtstr[32] = { 0 };
            sprintf(fmtstr,"%%0%uu.%%0%uu",digits,digits);
            snprintf(str,max_width-1,fmtstr,unsigned(size),unsigned(rank));

        }

    }

}
