#include "y/gfx/filter.hpp"

namespace upsylon
{

    namespace graphic
    {

        namespace crux
        {
            filter:: ~filter() throw()
            {
            }

            filter:: filter(const string &id) : entity(), name(id)
            {
            }

            filter:: filter(const char *id) : entity(), name(id)
            {
            }

        }

    }

}


