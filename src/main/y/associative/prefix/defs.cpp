

#include "y/associative/prefix/defs.hpp"
#include "y/code/utils.hpp"
#include <cstring>

namespace upsylon
{
    const char *prefix_::byte_to_text(const uint8_t &x) throw()
    {
        return cchars::printable[x];
    }

    char prefix_:: text[64] = { 0 };

    void prefix_:: clr() throw()
    {
        memset(text,0,sizeof(text));
    }

    void prefix_::cat(const char *id) throw()
    {
        strcat(text,id);
    }
}
