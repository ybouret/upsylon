#if 1
#include "y/lang/syntax/term-count.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            TermCount:: TermCount(const int lo, const int up) throw() : nmin(lo), nmax(up) {}

            TermCount:: TermCount(const TermCount &other) throw() :
            nmin( other.nmin ),
            nmax( other.nmax )
            {
            }

            TermCount:: ~TermCount() throw()
            {
            }

            TermCount & TermCount:: operator=(const TermCount &other) throw()
            {
                nmin = other.nmin;
                nmax = other.nmax;
                return *this;
            }

            std::ostream & operator<<( std::ostream &os, const TermCount &)
            {
                os << '[';
                os << ']';
                return os;
            }

        }
    }
}
#endif
