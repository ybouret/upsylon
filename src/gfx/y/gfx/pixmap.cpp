
#include "y/gfx/pixmap.hpp"
#include "y/type/rtti.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace crux
        {
            void pixmap:: invalid_depth(const unit_t          depth,
                                        const std::type_info &tid)
            {
                const string &id = rtti::name_of(tid);
                throw exception("pixmap<%s> invalid depth=%ld", *id, long(depth) );
            }

        }

    }
}


