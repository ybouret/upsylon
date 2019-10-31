#include "y/math/adjust/frame.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


            Frame:: ~Frame() throw()
            {
            }

            Frame:: Frame( const accessible<bool> &used_) throw() :
            used(used_)
            {
            }
            
        }

    }
}

