#include "y/information/filter/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            Context:: Context() throw() : last(-1)
            {
            }

            Context:: ~Context() throw()
            {
                last=-1;
            }

            void Context:: restart() throw()
            {
                last = -1;
                free();
            }
        }

    }
}
