#include "y/chemical/reactor.hpp"


namespace upsylon
{
    
    namespace Chemical
    {
        
        Reactor:: ~Reactor() throw()
        {}
        
        Reactor:: Reactor(Library       &usr_lib,
                          Equilibria    &usr_eqs,
                          const unsigned flags) :
        lib(usr_lib),
        eqs(usr_eqs),
        M(lib->size()),
        N(eqs->size()),
        NW(lib.countWorking()),
        NL(lib.countLeading()),
        NS(lib.countSeeking()),
        Nu(N>0?N:0,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        lockLib(lib),
        lockEqs(eqs)
        {
            //------------------------------------------------------------------
            //
            // setup
            //
            //------------------------------------------------------------------
            Y_CHEMICAL_PRINTLN("<Reactor>");
            Y_CHEMICAL_PRINTLN("  M  = " << M);
            Y_CHEMICAL_PRINTLN("  N  = " << N);
            Y_CHEMICAL_PRINTLN("  NW = " << NW);
            Y_CHEMICAL_PRINTLN("  NL = " << NL);
            Y_CHEMICAL_PRINTLN("  NS = " << NS);

            eqs.verify(flags);
            
            
            Y_CHEMICAL_PRINTLN("<Reactor/>");

            
        }
        
        
    }
}

