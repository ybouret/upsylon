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
            Y_ALCHEM_PRINTLN("  <Topology>");
            //__________________________________________________________________
            //
            //
            // initialize Nu and Nu'
            //
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            Y_ALCHEM_PRINTLN(" Nu     = " << Nu);
            Y_ALCHEM_PRINTLN(" NuT    = " << NuT);

            //__________________________________________________________________
            //
            //
            // checking consistency
            //
            //__________________________________________________________________
            if(N>0)
            {
                const size_t r = apk::rank(Nu);
                Y_ALCHEM_PRINTLN(" NuRank = " << r   << "/" << N);

                if(r<N)
                {
                    const unsigned long ns = static_cast<unsigned long>(N-r);
                    throw exception("%s has %lu redundant equilibri%s",CLID,ns,textual::plural_a(ns));
                }
            }
            Y_ALCHEM_PRINTLN("  <Topology/>");
        }

    }

}

