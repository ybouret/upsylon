

#include "y/associative/prefix/defs.hpp"
#include "y/code/utils.hpp"
#include "y/exceptions.hpp"

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

    void prefix_:: cat(const uint8_t b[], const size_t n) throw()
    {
        assert(n>0);
        const size_t top = n-1;
        size_t i=0;
        while(i<top&&b[i]<=0) ++i;
        while(i<n)
        {
            cat( byte_to_text(b[i]));
            ++i;
        }

    }

    void prefix_:: throw_unexpected_multiple_path()
    {
        throw exception("unexpectped multiple prefix path");
    }

}
