
#include "y/ios/endl.hpp"

namespace upsylon
{

    namespace ios
    {
        const cr_t   cr;
        const lf_t   lf;
        const crlf_t crlf;

        const char cr_t   :: expr[2] = { '\r', 0 };
        const char lf_t   :: expr[2] = { '\n', 0 };
        const char crlf_t :: expr[4] = { '\r', '\n', 0, 0 };

    }

}
