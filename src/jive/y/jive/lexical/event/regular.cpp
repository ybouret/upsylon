
#include "y/jive/lexical/event/regular.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {


            RegularEvent:: ~RegularEvent() throw()
            {
            }

            RegularEvent:: RegularEvent(const Type    t,
                                        const Action &a) :
            Event(Regular,a), type(t)
            {
            }

            const void * RegularEvent:: self() const throw()
            {
                return static_cast<const RegularEvent *>(this);
            }



        }

    }

}
