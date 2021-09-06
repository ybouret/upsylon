
#include "y/chemical/system.hpp"


namespace upsylon
{
    namespace Chemical
    {

        System:: ~System() throw()
        {
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(eqs->size()),
        M(eqs->size()),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {

            Y_CHEMICAL_PRINTLN("<System>");
            Y_CHEMICAL_PRINTLN("  N = " << N);
            Y_CHEMICAL_PRINTLN("  M = " << M);

            if(N>0)
            {
                eqs.verify(flags);
                eqs.fill( aliasing::_(Nu) );
                aliasing::_(NuT).assign_transpose(Nu);
                
                for(size_t i=1;i<=N;++i)
                {
                }
            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }


    }

}
