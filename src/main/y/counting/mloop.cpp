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
        }

        static inline size_t check_loop( const size_t dims )
        {
            if(dims<=0) throw exception("%sno dimension", mloop_::identifier );
            return dims;
        }

        mloop_:: mloop_(const size_t dims,
                        const bool   safe):
        counting(0,check_loop(dims)),
        secured(safe)
        {
        }

        mloop_:: mloop_(const mloop_ &other) throw() :
        counting(other),
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
