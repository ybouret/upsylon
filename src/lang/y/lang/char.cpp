#include "y/lang/char.hpp"

namespace upsylon
{
    namespace Lang
    {

        Manager:: Manager() throw() : singleton<Manager>(), Char::Pool()
        {
        }

        Manager:: ~Manager() throw()
        {
        }


    }

}

