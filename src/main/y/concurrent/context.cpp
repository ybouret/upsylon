#include "y/concurrent/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/ints-utils.hpp"
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
            char *str = (char *)label;
            memset(str,0,sizeof(label));
            snprintf(str,sizeof(label)-1,"%u.%u", unsigned(size), unsigned(rank) );
        }

    }

}
