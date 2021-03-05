
#include "y/gfx/pixmap.hpp"
#include "y/type/spec.hpp"
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
                const string &id = type_name_for(tid);
                throw exception("pixmap<%s> invalid depth=%ld", *id, long(depth) );
            }

        }

    }
}


