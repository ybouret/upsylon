
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/unique.hpp"
#include "y/core/dnode.hpp"

#include "y/counting/comb.hpp"

#include <iomanip>

namespace upsylon
{

    namespace Chemical
    {

        Lineage:: ~Lineage() throw()
        {
        }

        Lineage:: Lineage( const Species &sp ) :
        authority<const Species>(sp),
        bounded(true),
        primary(sp.rating,as_capacity)
        {

        }


        void Lineage:: link(const Primary::Pointer &p) throw()
        {
            aliasing::_(primary).push_back_(p);
            if(!p->keep)
            {
                aliasing::_(bounded) = false;
            }
        }

    }

}


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
        lineage(M,as_capacity),
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
            Y_CHEMICAL_PRINTLN("  Nc  = " << std::setw(3) << Nc << " # complementaries");
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

                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;

                
                //--------------------------------------------------------------
                //
                // building lineage
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  <Lineage>");
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species     &sp = ***node;
                    const iAccessible &nu = NuT[sp.indx];
                    Lineage           *l = new Lineage(sp);
                    Lineage::Pointer  lp = l;
                    std::cerr << "    " << sp << " #" << std::setw(3) << sp.rating << " :";
                    aliasing::_(lineage).push_back_(lp);
                    for(size_t i=1;i<=N;++i)
                    {
                        if(nu[i])
                        {
                            l->link(primary[i]);
                            std::cerr << ' ' << (**primary[i]).name;
                        }
                    }
                    
                    std::cerr << " => " << Primary::KeepText(l->bounded) << std::endl;

                }

                Y_CHEMICAL_PRINTLN("  <Lineage/>");

                buildOmega();



            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        typedef vector<apq,Allocator> qVector;
        typedef arc_ptr<qVector>      qShared;

        void System::buildOmega()
        {
            Y_CHEMICAL_PRINTLN("  <Omega>");
            if(M>N)
            {
                Flags         alive(M,true);
                size_t        dim = M-N;
                list<qShared> Omega;

                //--------------------------------------------------------------
                //
                // spectators and endless species setup
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    const Lineage &l = *lineage[j];
                    if(l->rating<=0)
                    {
                        assert(l.bounded);
                        qShared Q = new qVector(M,0);
                        Omega.push_back(Q);
                        (*Q)[j]   = 1;
                        alive[j]  = false;
                        --dim;
                    }
                    else
                    {
                        if(!l.bounded)
                        {
                            alive[j] = false;
                        }
                    }

                }
                assert(Nc==dim);

                std::cerr << "Omega=" << Omega << std::endl;
            }
            Y_CHEMICAL_PRINTLN("  <Omega/>");

        }
    }

}
