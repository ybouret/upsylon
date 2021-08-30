#include "y/chemical/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {
        
        const char Reactor:: CLID[] = "Chemical::Reactor";
        
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
        leading(N,as_capacity),
        lockLib(lib),
        lockEqs(eqs)
        {
            //------------------------------------------------------------------
            //
            // setup
            //
            //------------------------------------------------------------------
            Y_CHEMICAL_PRINTLN("<Reactor>");
            Y_CHEMICAL_PRINTLN("  M   = " << M);
            Y_CHEMICAL_PRINTLN("  N   = " << N);
            Y_CHEMICAL_PRINTLN("  NW  = " << NW);
            Y_CHEMICAL_PRINTLN("  NL  = " << NL);
            Y_CHEMICAL_PRINTLN("  NS  = " << NS);
            
            //------------------------------------------------------------------
            //
            // check topology
            //
            //------------------------------------------------------------------
            eqs.verify(flags);
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            const size_t rankNu = apk::rank(Nu);

            Y_CHEMICAL_PRINTLN("  Nu  = " << Nu);
            Y_CHEMICAL_PRINTLN("  NuT = " << NuT);
            Y_CHEMICAL_PRINTLN("      |_rank= " << rankNu);
            if(rankNu<N) throw exception("%s equilibria are not independent",CLID);
            
            //------------------------------------------------------------------
            //
            // create leading conditions
            //
            //------------------------------------------------------------------
            for(const ENode *node=eqs->head();node;node=node->next)
            {
                const Leading::Pointer p( new Leading(**node) );
                aliasing::_(leading).push_back_(p);
            }

            if(Verbosity)
            {
                showLeading(std::cerr);
            }

            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            Y_CHEMICAL_PRINTLN("<Reactor/>");
        }
        
        
    }
}

