
#include "y/gfx/bitrow.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace graphic
    {
        bitrow:: ~bitrow() throw()
        {
            p=0;
            _bzset(w);
        }

        bitrow:: bitrow(void        *entry,
                        const unit_t width,
                        const zflux &zhost) throw() :
        p(entry),
        w(width),
        z(zhost)
        {
        }

        void *bitrow:: at(const unit_t i, const unit_t depth) throw()
        {
            assert(i>=0); assert(i<w);
            return static_cast<char *>(p)+depth*i;
        }

        const void *bitrow:: at(const unit_t i, const unit_t depth) const throw()
        {
            assert(i>=0); assert(i<w);
            return static_cast<const char *>(p)+depth*i;
        }


    }

}


