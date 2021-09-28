#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/code/textual.hpp"
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
        size_t checkSizes(const size_t M, const size_t MW, const size_t N)
        {
            if(N>MW) throw exception("%s has too many equilibria/working species",System::CLID);
            return M-N;
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(  eqs->size() ),
        M(  lib->size() ),
        MW( lib.countWorking()  ),
        Nc( checkSizes(M,MW,N)  ),
        MP( lib.countPrimary()  ),
        MR( lib.countReplica()  ),
        MS( lib.spectators()    ),
        ME( 0 ),
        MB( 0 ),
        Nu(N,N>0?M:0),
        NuT(Nu,matrix_transpose),
        primary(N,as_capacity),
        lineage(M,as_capacity),
        replica(MR,as_capacity),
        Pproj(MP,MP>0?M:0),
        Rproj(MR,MR>0?M:0),
        NuP(MP>0?N:0,MP),
        NuPT(NuP,matrix_transpose),
        NuR(MR>0?N:0,MR),
        NuRT(NuR,matrix_transpose),
        Z(M,0),
        charged(false),
        Omega(Nc,Nc>0?M:0),
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

            assert(N+Nc==M);
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
                    const size_t rankNu = apk::rank(Nu);
                    Y_CHEMICAL_PRINTLN("  rankNu  = " << rankNu);
                    if(rankNu<N)
                    {
                        throw exception("%s has singular equilibria (rank=%u/%u)",CLID, unsigned(rankNu), unsigned(N) );
                    }
                }



                //--------------------------------------------------------------
                //
                // building primary
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << PrimaryEnter);
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium     &eq = ***node;
                    const Primary::Pointer pp = new Primary(eq,NuT);
                    aliasing::_(primary).push_back_(pp);
                    if(Verbosity) pp->display(std::cerr,4);

                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);


                //--------------------------------------------------------------
                //
                // building replica
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << ReplicaEnter);
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    if(!sp.isReplica()) continue;
                    const ENode               *en = eqs->head();
                    const iAccessible         &nu = NuT[sp.indx];
                    while(!nu[ (***en).indx] ) en = en->next;
                    const Replica::Pointer     rp = new Replica(sp,nu,en);
                    aliasing::_(replica).push_back_(rp);
                    rp->display(std::cerr,4);
                    
                }
                Y_CHEMICAL_PRINTLN("  " << ReplicaLeave);

            }

            if(true)
            {
                //--------------------------------------------------------------
                //
                // building lineage
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  <Lineage>");

                Y_CHEMICAL_PRINTLN("    <Building>");
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species         &s = ***node;
                    Lineage               *S = new Lineage(s);
                    { const Lineage::Pointer  tmp(S); aliasing::_(lineage).push_back_(tmp); }
                    const size_t           j = s.indx;


                    if(Verbosity) std::cerr << "      " << s << " #" << std::setw(3) << s.rating << " :";

                    for(size_t i=1;i<=N;++i)
                    {
                        const unit_t nu = NuT[j][i];
                        if(NuT[j][i]!=0)
                        {
                            const Primary &p = *primary[i];
                            S->link(nu,p);
                            if(Verbosity) std::cerr << ' ' << p->name;
                        }
                    }
                    S->finalize();
                    if(s.rating>0)
                    {
                        if(0!=(aliasing::_(Z[j]) = s.charge))
                        {
                            aliasing::_(charged) = true;
                        }
                        switch(S->state)
                        {
                            case Flow::Endless: aliasing::incr(ME); break;
                            case Flow::Bounded: aliasing::incr(MB); break;
                        }
                    }
                    if(Verbosity) std::cerr << " => " << S->stateText() << "/" << S->linkageText() << std::endl;
                }
                Y_CHEMICAL_PRINTLN("     Z = " << Z << " #charged=" << textual::boolean(charged) );
                Y_CHEMICAL_PRINTLN("     ENDLESS = " << ME);
                Y_CHEMICAL_PRINTLN("     BOUNDED = " << MB);
                Y_CHEMICAL_PRINTLN("     SINGLE  = " << MS);
                assert(MW==ME+MB);
                Y_CHEMICAL_PRINTLN("    <Building/>");

                Y_CHEMICAL_PRINTLN("    <Compiled>");
                if(Verbosity)
                {
                    for(size_t i=1;i<=M;++i)
                    {
                        std::cerr << "      " << *lineage[i] << std::endl;
                    }
                }
                Y_CHEMICAL_PRINTLN("    <Compiled/>");


                Y_CHEMICAL_PRINTLN("  <Lineage/>");
            }


            if(true)
            {
                //--------------------------------------------------------------
                //
                // building projection
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  <Projection>");

                for(size_t j=1,jp=0,jr=0;j<=M;++j)
                {

#if 1
                    switch( (**lineage[j]).rating )
                    {
                        case 0: break;
                        case 1:  aliasing::_(Pproj[++jp][j]) = 1; break;
                        default: aliasing::_(Rproj[++jr][j]) = 1; break;
                    }
#endif
                }

                tao::mmul( aliasing::_(NuPT),Pproj,NuT);
                aliasing::_(NuP).assign_transpose(NuPT);

                tao::mmul( aliasing::_(NuRT),Rproj,NuT);
                aliasing::_(NuR).assign_transpose(NuRT);

                Y_CHEMICAL_PRINTLN("    Nu    = " << Nu);
                Y_CHEMICAL_PRINTLN("    NuT   = " << NuT);
                Y_CHEMICAL_PRINTLN("    Pproj = " << Pproj);
                Y_CHEMICAL_PRINTLN("    Rproj = " << Rproj);
                Y_CHEMICAL_PRINTLN("    NuP   = " << NuP);
                Y_CHEMICAL_PRINTLN("    NuPT  = " << NuPT);
                Y_CHEMICAL_PRINTLN("    NuR   = " << NuR);
                Y_CHEMICAL_PRINTLN("    NuRT  = " << NuRT);


                Y_CHEMICAL_PRINTLN("  <Projection/>");

            }

            if(false)
            {
                graphViz("endless.dot",false);
                graphViz("bounded.dot",true);
            }

            graphViz("system.dot",false);

            //buildClusters();
            buildOmega();
            

            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

