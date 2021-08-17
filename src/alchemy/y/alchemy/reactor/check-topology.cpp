#include "y/alchemy/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/exception.hpp"
#include "y/code/textual.hpp"


namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        void Reactor:: checkTopology()
        {
            std::cerr << "  <Topology>" << std::endl;
            //__________________________________________________________________
            //
            //
            // initialize Nu and Nu'
            //
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            std::cerr << " Nu     = " << Nu  << std::endl;
            std::cerr << " NuT    = " << NuT << std::endl;

            //__________________________________________________________________
            //
            //
            // checking consistency
            //
            //__________________________________________________________________
            if(N>0)
            {
                const size_t r = apk::rank(Nu);
                std::cerr << " NuRank = " << r   << "/" << N << std::endl;

                if(r<N)
                {
                    const unsigned long ns = static_cast<unsigned long>(N-r);
                    throw exception("%s has %lu redundant equilibri%s",CLID,ns,textual::plural_a(ns));
                }
            }
            std::cerr << "  <Topology/>" << std::endl;
        }

    }

}

