
#include "y/jive/lexical/editor.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            Editor:: ~Editor() throw()
            {
            }

            Editor:: Editor() : rules(), hoard(), intro(), outro()
            {
                aliasing::_(outro).complete();
            }


        }

    }

}
