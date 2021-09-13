
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/unique.hpp"
#include "y/core/dnode.hpp"

#include "y/ios/tools/vizible.hpp"
#include "y/string/convert.hpp"

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
            Y_CHEMICAL_PRINTLN("  N   = " << N);
            Y_CHEMICAL_PRINTLN("  M   = " << M);
            Y_CHEMICAL_PRINTLN("  MW  = " << MW);
            Y_CHEMICAL_PRINTLN("  MP  = " << MP);
            Y_CHEMICAL_PRINTLN("  MR  = " << MR);
            Y_CHEMICAL_PRINTLN("  MS  = " << MS);

            
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
                        for(const ANode *a=eq.reac->head();a;a=a->next)
                        {
                            const Species &sp = (**a).sp;
                            aliasing::_(bounded)[sp.indx] = false;
                        }
                        for(const ANode *a=eq.prod->head();a;a=a->next)
                        {
                            const Species &sp = (**a).sp;
                            aliasing::_(bounded)[sp.indx] = false;
                        }

                    }
                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;
                if(Verbosity) lib.display(std::cerr<<"  bounded =",bounded,2) << std::endl;

                {
                    const size_t dim = M-N;
                    if(dim>0)
                    {
                        iMatrix Omega(dim,M);
                        {
                            size_t  i=dim;
                            for(const SNode *node=lib->head();node;node=node->next)
                            {
                                const Species &sp = ***node;
                                if(sp.rating<=0)
                                {
                                    Omega[i--][sp.indx] = 1;
                                }
                            }
                        }
                        std::cerr << "Omega=" << Omega << std::endl;
                    }

                }

                
                



            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

