#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
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
        primary(N,as_capacity),
        strain(M, as_capacity),
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

            }

            {
                //--------------------------------------------------------------
                //
                // building strains
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  <Strains>");

                Y_CHEMICAL_PRINTLN("    <Building>");
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species         &s = ***node;
                    Strain                *S = new Strain(s);
                    { const Strain::Pointer  tmp(S); aliasing::_(strain).push_back_(tmp); }
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
                    if(Verbosity) std::cerr << " => " << S->stateText() << std::endl;

                }
                Y_CHEMICAL_PRINTLN("    <Building/>");

                Y_CHEMICAL_PRINTLN("    <Compiled>");
                if(Verbosity)
                {
                    for(size_t i=1;i<=M;++i)
                    {
                        std::cerr << "      " << *strain[i] << std::endl;
                    }
                }
                Y_CHEMICAL_PRINTLN("    <Compiled/>");


                Y_CHEMICAL_PRINTLN("  <Strains/>");
            }
            
           // buildOmega();


            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

