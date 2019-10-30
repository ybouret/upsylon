
#include "y/math/adjust/share/common.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


            Common:: ~Common() throw()
            {
            }

            Common:: Common(const size_t n) : names(n,as_capacity)
            {
            }
            
            void Common:: appendTo( Variables &vars ) const
            {
                for(size_t i=1;i<=names.size();++i)
                {
                    vars << names[i];
                }
            }

            string Common:: Suffix(const size_t value)
            {
                return vformat("%lu", static_cast<unsigned long>(value));
            }
        }
    }
}


