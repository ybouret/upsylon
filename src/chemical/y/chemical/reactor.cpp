#include "y/chemical/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/code/textual.hpp"

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
        NuL(Nu.rows,Nu.cols),
        NuLT(NuT.rows,NuT.cols),
        NuS(Nu.rows,Nu.cols),
        NuST(NuT.rows,NuT.cols),
        leading(N,as_capacity),
        seeking(NS,as_capacity),
        xi(N,0),
        Vs(NS,NS>0?N:0),
        VsT(Vs.cols,Vs.rows),
        IV2(NS,NS),
        Cs(NS,0),
        Rs(NS,0),
        xs(N,0),
        // private
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

            Y_CHEMICAL_PRINTLN("  Nu   = " << Nu);
            Y_CHEMICAL_PRINTLN("  NuT  = " << NuT);
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


            for(size_t i=N;i>0;--i)
            {
                const Leading &l = *leading[i];
                const size_t   I = l.root->indx;
                for(size_t j=l.reac.size();j>0;--j)
                {
                    const size_t J = l.reac[j].sp.indx;
                    aliasing::_(NuL[I][J]) = Nu[I][J];
                }
                for(size_t j=l.prod.size();j>0;--j)
                {
                    const size_t J = l.prod[j].sp.indx;
                    aliasing::_(NuL[I][J]) = Nu[I][J];
                }
            }
            aliasing::_(NuLT).assign_transpose(NuL);
            Y_CHEMICAL_PRINTLN("  NuL  = " << NuL);
            Y_CHEMICAL_PRINTLN("  NuLT = " << NuLT);

            //------------------------------------------------------------------
            //
            // create seeking conditions
            //
            //------------------------------------------------------------------
            for(const SNode *node=lib->head();node;node=node->next)
            {
                const Species &sp = ***node;
                if(sp.rating>1)
                {
                    const Seeking::Pointer p( new Seeking(sp,NuT,eqs->head()) );
                    aliasing::_(seeking).push_back_(p);
                    tao::set(aliasing::_(NuST[sp.indx]),p->nu);
                }
            }
            aliasing::_(NuS).assign_transpose(NuST);
            Y_CHEMICAL_PRINTLN("  NuS  = " << NuS);
            Y_CHEMICAL_PRINTLN("  NuST = " << NuST);



            if(Verbosity)
            {
                showLeading(std::cerr);
                showSeeking(std::cerr);
            }

            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            Y_CHEMICAL_PRINTLN("<Reactor/>");
        }
        
        bool Reactor:: balance(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("<Balance>");
            bool result = balanceLeading(C);
            if(result)
            {
                result = balanceSeeking(C);
            }
            Y_CHEMICAL_PRINTLN("  [balanced=" << textual::boolean(result) << "]" );
            Y_CHEMICAL_PRINTLN("<Balance/>");
            return result;
        }
    }
}

