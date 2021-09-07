
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        const char System::CLID[] = "Chemical::System";

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
        size_t checkValidity(const Library &lib, const Equilibria &eqs)
        {
            const size_t M = lib->size();
            if(M<eqs->size()) throw exception("%s has too many equilibria",System::CLID);
            return M;
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(eqs->size()),
        M( checkValidity(lib,eqs) ),
        NP(0),
        NR(0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        primary(N,as_capacity),
        replica(N,as_capacity),
        xi(N,0),
        ok(N,false),
        Vr(),
        Ur(),
        V2(),
        Cr(),
        xr(),
        go(),
        ix(),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {

            Y_CHEMICAL_PRINTLN("<System>");
            Y_CHEMICAL_PRINTLN("  N   = " << N);
            Y_CHEMICAL_PRINTLN("  M   = " << M);

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
                    const Primary::Pointer pp = new Primary(***node,NuT);
                    aliasing::_(primary).push_back_(pp);
                    aliasing::_(NP) += pp->count();
                    if(Verbosity) pp->display(std::cerr,4);
                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;
                Y_CHEMICAL_PRINTLN("  NP  = " << NP);

                //--------------------------------------------------------------
                //
                // building replica
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << ReplicaEnter);;
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    if(sp.rating>1)
                    {
                        const iAccessible     &nu = NuT[sp.indx];
                        const ENode           *en = eqs->head(); while( !nu[ (***en).indx] ) en = en->next;
                        const Replica::Pointer rp = new Replica(sp,nu,en);
                        aliasing::_(replica).push_back_(rp);
                        aliasing::incr(NR);
                        if(Verbosity) rp->display(std::cerr,4);
                    }
                }
                Y_CHEMICAL_PRINTLN("  " << ReplicaLeave);;
                Y_CHEMICAL_PRINTLN("  NR  = " << NR);
                if(NR>0)
                {
                    Vr.make(NR,N);
                    Ur.make(N,NR);
                    V2.make(NR,NR);
                    Cr.make(NR,0);
                    Br.make(NR,0);
                    xr.make(N,0);
                    go.make(N,false);
                    ix.make(N,0);
                }
            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }


    }

}

