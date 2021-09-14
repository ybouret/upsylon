
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/unique.hpp"
#include "y/core/dnode.hpp"

#include "y/ios/tools/vizible.hpp"
#include "y/string/convert.hpp"
#include "y/counting/comb.hpp"

#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        const char System:: CLID[] = "Chemical::System";
        const char System:: PrimaryEnter[] = "<Primary>";
        const char System:: PrimaryLeave[] = "<Primary/>";
        const char System:: ReplicaEnter[] = "<Replica>";
        const char System:: ReplicaLeave[] = "<Replica/>";

        const char System:: Success[] = "Success";
        const char System:: Failure[] = "Failure";
        
        const char * System:: Outcome(const bool flag) throw()
        {
            return flag ? Success : Failure;
        }

        System:: ~System() throw()
        {
        }


        static inline
        size_t checkValidity(const size_t MW, const size_t N)
        {
            if(N>MW) throw exception("%s has too many equilibria/working species",System::CLID);
            return MW-N;
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(  eqs->size() ),
        M(  lib->size() ),
        MW( lib.countWorking()  ),
        Nc( checkValidity(MW,N) ),
        MP( lib.countPrimary()  ),
        MR( lib.countReplica()  ),
        MS( lib.spectators()    ),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        primary(N, as_capacity),
        bounded(M,true),
        xi(N,0),
        ok(N,false),
        who(N,as_capacity),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {

            Y_CHEMICAL_PRINTLN("<System>");
            Y_CHEMICAL_PRINTLN("  N   = " << std::setw(3) << N  << " # equilibria");
            Y_CHEMICAL_PRINTLN("  M   = " << std::setw(3) << M  << " # species");
            Y_CHEMICAL_PRINTLN("  MW  = " << std::setw(3) << MW << " # working species");
            Y_CHEMICAL_PRINTLN("  Nc  = " << std::setw(3) << Nc << " # conservations");
            Y_CHEMICAL_PRINTLN("  MP  = " << std::setw(3) << MP << " # primary species");
            Y_CHEMICAL_PRINTLN("  MR  = " << std::setw(3) << MR << " # replica species");
            Y_CHEMICAL_PRINTLN("  MS  = " << std::setw(3) << MS << " # spectators");

            assert(N+Nc==MW);
            assert(MP+MR==MW);
            assert(MW+MS==M);

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // checking equilibria
                //
                //--------------------------------------------------------------
                eqs.verify(flags);
                eqs.fill( aliasing::_(Nu) );
                aliasing::_(NuT).assign_transpose(Nu);
                Y_CHEMICAL_PRINTLN("  Nu  = " << Nu);
                Y_CHEMICAL_PRINTLN("  NuT = " << NuT);

                {
                    Matrix Nu2(N,N);
                    tao::gram(Nu2,Nu);
                    if(!LU::build(Nu2))
                    {
                        throw exception("%s has singular equilibria",CLID);
                    }
                }

                //--------------------------------------------------------------
                //
                // building primary
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << PrimaryEnter);;
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium     &eq = ***node;
                    const Primary::Pointer pp = new Primary(eq,NuT);
                    aliasing::_(primary).push_back_(pp);
                    if(Verbosity) pp->display(std::cerr,4);
                    if(!pp->keep)
                    {
                        for(const CNode *sub=eq.used.head();sub;sub=sub->next)
                        {
                            const Compound &cmp = ***sub;
                            aliasing::_(bounded[cmp->sp.indx]) = false;
                        }
                    }
                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;
                if(Verbosity) lib.display(std::cerr<<"  bounded =",bounded,2) << std::endl;

                
                


                
                



            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

