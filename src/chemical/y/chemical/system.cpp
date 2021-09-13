
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
        MW( lib.countWorking() ),
        MP( lib.countPrimary() ),
        MR( lib.countReplica() ),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        primary(N, as_capacity),
        replica(MR,as_capacity),
        xi(N,0),
        ok(N,false),
        who(N,as_capacity),
        ix(N,0),
        go(N,false),
        xv(N,as_capacity),
        Cr(),
        Br(),
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
                    //aliasing::_(NP) += pp->count();
                    if(Verbosity) pp->display(std::cerr,4);
                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;
                Y_CHEMICAL_PRINTLN("  MP  = " << MP);

                //--------------------------------------------------------------
                //
                // building replica
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << ReplicaEnter);
                Indices ratings(M,as_capacity);
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    const size_t   sr = sp.rating;
                    if(sp.rating>1)
                    {
                        const iAccessible     &nu = NuT[sp.indx];
                        const ENode           *en = eqs->head(); while( !nu[ (***en).indx] ) en = en->next;
                        const Replica::Pointer rp = new Replica(sp,nu,en);
                        aliasing::_(replica).push_back_(rp);
                        if(Verbosity) rp->display(std::cerr,4);
                    }
                    ratings.push_back_(sr);
                }
                Y_CHEMICAL_PRINTLN("  " << ReplicaLeave);;
                Y_CHEMICAL_PRINTLN("  MR  = " << MR);
                Y_CHEMICAL_PRINTLN("  ratings  = " << ratings);
                unique(ratings);
                Y_CHEMICAL_PRINTLN("  ratings  = " << ratings);


                if(MR>0)
                {
                    Matrix Vr(MR,N);
                    for(size_t i=MR;i>0;--i)
                    {
                        tao::set(Vr[i],replica[i]->nu);
                    }
                    std::cerr << "Vr=" << Vr << std::endl;
                    Cr.make(MR,0);
                    Br.make(MR,0);
                }


                



            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }

        bool System:: balance(Addressable &C) throw()
        {
            Y_CHEMICAL_PRINTLN("<Balance>");
            bool success = balancePrimary(C);
            if( success )
            {
                success = balanceReplica(C);
            }
            Y_CHEMICAL_PRINTLN("<Balance/>");
            return success;
        }

    }

}

