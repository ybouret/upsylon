
#include "y/code/textual.hpp"

namespace upsylon
{

    const char * textual:: plural_s(const size_t count) throw()
    {
        return (count<=1) ? "" : "s";
    }

    const char * textual:: boolean(const bool flag) throw()
    {
        return flag ? "true" : "false";
    }
}


