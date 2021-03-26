#include "y/gfx/color/ramp/tableau.hpp"


namespace upsylon
{

    namespace graphic
    {

        tableau:: ~tableau() throw()
        {
        }

        tableau:: tableau(const rgba * const rp, const size_t sz) throw() :
        repository_(rp),
        dimensions_(sz)
        {

        }

        size_t       tableau:: dimensions() const throw() { return dimensions_; }
        const rgba * tableau:: repository() const throw() { return repository_; }
        
    }

}

#include "y/gfx/color/named.hpp"

namespace upsylon
{

    namespace graphic
    {
        const rgba cold_to_hot:: repo[ dims ] =
        {
            Y_BLUE,
            Y_CYAN,
            Y_GREEN,
            Y_YELLOW,
            Y_RED
        };
    }

}
