
#include "y/chemical/reactor.hpp"
#include "y/code/textual.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {
        bool Reactor:: balanceSeeking(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("  <Balance Seeking>");
            bool result = true;
            if(NS>0)
            {
                if(Verbosity)
                {
                    lib.display(std::cerr,C) << std::endl;
                }
                //--------------------------------------------------------------
                //
                // initialize full search
                //
                //--------------------------------------------------------------
                for(size_t i=NS;i>0;--i)
                {
                    tao::set(NuS[i],seeking[i]->nu);
                }
                tao::gram(NuS2,NuS);
                Y_CHEMICAL_PRINTLN("    NuS  = " << NuS);
                Y_CHEMICAL_PRINTLN("    NuS2 = " << NuS2);


            }

            Y_CHEMICAL_PRINTLN("    [seeking balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("  <Balance Seeking/>");
            return result;
        }

    }

}

