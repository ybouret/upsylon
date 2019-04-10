
#include "y/mpl/pfq.hpp"
#include <cerrno>
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace mpl
    {

        pfq:: ~pfq() throw()
        {
        }

        pfq:: pfq() :
        n(0),d(1)
        {
        }

        std::ostream & operator<<( std::ostream &os, const pfq &Q)
        {
            const pfd &n = Q.n;
            const pfd &d = Q.d;
            if(n.is_zero())
            {
                assert(d.is_one());
                os << '0';
            }
            else
            {
                if(d.is_one())
                {
                    os << n;
                }
                else
                {
                    os << n << '/' << d;
                }
            }
            return os;
        }
        

    }

}

