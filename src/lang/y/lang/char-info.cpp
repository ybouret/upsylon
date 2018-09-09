
#include "y/lang/char-info.hpp"

namespace upsylon
{
    namespace Lang
    {
        CharInfo:: ~CharInfo() throw() {}

        CharInfo:: CharInfo(const Origin &org, const int l, const int c) throw() :
        origin(org),
        line(l),
        column(c)
        {
        }

        CharInfo:: CharInfo( const CharInfo &other ) throw() :
        origin( other.origin ),
        line(   other.line   ),
        column( other.column )
        {
        }

        void CharInfo:: swapWith( CharInfo &other ) throw()
        {
            cswap(origin,other.origin);
            cswap(line,other.line);
            cswap(column,other.column);
        }

        CharInfo & CharInfo:: operator=( const CharInfo &other ) throw()
        {
            CharInfo tmp(other);
            swapWith(tmp);
            return *this;
        }

    }
}
