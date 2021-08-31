#include "y/chemical/reactor.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{

    namespace Chemical
    {
        bool Reactor:: balanceLeading(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("  <Balance Leading>");
            if(Verbosity)
            {
                lib.display(std::cerr << "C0=",C) << std::endl;
            }
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

            Y_CHEMICAL_PRINTLN("    [ ==> leading balanced=" << textual::boolean(result) << " ]" );
            Y_CHEMICAL_PRINTLN("  <Balance Leading/>");
            return result;
        }


    }

}

