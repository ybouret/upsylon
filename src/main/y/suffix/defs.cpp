
#include "y/suffix/defs.hpp"
#include "y/code/utils.hpp"
#include <cstring>

namespace upsylon
{
    void *suffix:: in_use() throw()
    {
        static const union {
            flag_t f;
            void  *p;
        } alias = { unsigned_int<flag_size>::maximum };
        return alias.p;
    }

    const char *suffix::byte_to_text(const uint8_t &x) throw()
    {
        return cchars::printable[x];
    }

    char suffix:: text[64] = { 0 };

    void suffix:: clr() throw()
    {
        memset(text,0,sizeof(text));
    }

    void suffix::cat(const char *id) throw()
    {
        strcat(text,id);
    }

    void suffix:: cat(const uint8_t b[], const size_t n) throw()
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


}
