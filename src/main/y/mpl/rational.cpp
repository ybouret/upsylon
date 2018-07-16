#include "y/mpl/rational.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include "y/code/arith.hpp"

namespace upsylon
{
    namespace mpl
    {
        void rational:: check()
        {
            if(den.is_zero())
            {
                throw libc::exception(EDOM,"mpl.rational: nul denominator");
            }
            __simplify();
        }

        void rational:: __simplify()
        {
            assert(den>0);
            natural &d = (natural &) den;
            if(!d.is_byte(1))
            {
                natural &n = (natural &)(num.n);
                arithmetic::simplify(n,d);
            }
        }
    }
}
