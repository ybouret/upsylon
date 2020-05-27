#include "y/spade/layout/tessellation.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade
    {

        namespace Kernel {

            Tessellation:: ~Tessellation() throw() {}

            Tessellation:: Tessellation(const unsigned dim) throw() : dimensions(dim)
            {
            }

            void Tessellation:: throwNoMapping(const unsigned dim)
            {
                throw exception("Spade::Tessellation%ud: no mapping!",dim);
            }

        }
    }

}


