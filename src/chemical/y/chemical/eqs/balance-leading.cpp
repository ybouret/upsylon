#include "y/chemical/reactor.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{

    namespace Chemical
    {
        bool Reactor:: balanceLeading(Addressable &C) const throw()
        {
            Vector xi(N,0);
            Y_CHEMICAL_PRINTLN("  <Balance Leading>");
            bool result = true;
            for(size_t i=N;i>0;--i)
            {
                switch( leading[i]->solve(C, NuT, xi) )
                {
                    case Leading::Modified:
                    case Leading::Accepted:
                        break;
                    case Leading::Rejected:
                        result = false;
                        break;
                }
            }
            Y_CHEMICAL_PRINTLN("    [" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Leading/>");
            return false;
        }


    }

}

