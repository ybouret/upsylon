#include "y/counting/mloop.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace core
    {
        const char mloop_:: identifier[]   = "mloop: ";
        const char mloop_:: separators[2] = { ',', 0 };

        mloop_:: ~mloop_() throw()
        {
            _bzset(dimensions);
        }

        mloop_:: mloop_(const size_t dims,
                        const bool   safe) throw():
        counting(0,dims),
        dimensions(dims),
        secured(safe)
        {
            assert(dimensions>0); //! checked by counting chkdim
        }

        mloop_:: mloop_(const mloop_ &other) throw() :
        counting(other),
        dimensions(other.dimensions),
        secured(other.secured)
        {
        }

        void mloop_:: overrule()   const
        {
            if(secured)
            {
                if(index<=1)     return;
                if(index>=count) return;
                throw exception("%ssecured loop cannot be changed at that point",identifier);
            }
        }

    }
}
