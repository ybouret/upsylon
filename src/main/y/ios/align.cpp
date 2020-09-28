#include "y/ios/align.hpp"

namespace upsylon
{
    namespace ios
    {
        align:: ~align() throw()
        {
        }

        align:: align(const align &other) throw() :
        text(other.text),
        size(other.size),
        kind(other.kind),
        width(other.width)
        {
        }


        align:: align(const char *s, const size_t n, const justify p, const size_t w) throw() :
        text(s),
        size(n),
        kind(p),
        width(w)
        {
            assert( !(NULL==s&&n>0) );
        }

        align:: align(const char   *s, const justify p, const size_t w) throw() :
        text(s),
        size( length_of(s) ),
        kind(p),
        width(w)
        {
        }

        align:: align(const string &s, const justify p, const size_t w) throw() :
        text(*s),
        size(s.size()),
        kind(p),
        width(w)
        {
        }

        align:: align(const char &C, const justify p, const size_t w) throw() :
        text(&C),
        size(1),
        kind(p),
        width(w)
        {
        }
        

    }
}

