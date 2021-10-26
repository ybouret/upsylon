
#include "y/chemical/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;
    
    namespace Chemical
    {
        Reactor:: ~Reactor() throw()
        {
        }
        
        const char Reactor:: CLID[] = "Reactor";
        
        static inline
        size_t checkSizes(const size_t M, const size_t MW, const size_t N)
        {
            if(N>MW) throw exception("%s has too many equilibria/working species",Reactor::CLID);
            return M-N;
        }
        Reactor:: Reactor(const Library    &usrLib,
                          const Equilibria &usrEqs,
                          const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(  eqs->size() ),
        M(  lib->size() ),
        MW( lib.countWorking()  ),
        Nc( checkSizes(M,MW,N)  ),
        MS( lib.spectators()    ),
        Nu(N,N>0?M:0),
        NuT(Nu,matrix_transpose),
        Z(M,0),
        K(N,0),
        Gamma(N,0),
        J(Nu.rows,Nu.cols),
        charged(false),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {
            
            Y_CHEMICAL_PRINTLN("<Reactor>");
            Y_CHEMICAL_PRINTLN("  N   = " << std::setw(3) << N  << " # equilibria");
            Y_CHEMICAL_PRINTLN("  M   = " << std::setw(3) << M  << " # species");
            Y_CHEMICAL_PRINTLN("  MW  = " << std::setw(3) << MW << " # working species");
            Y_CHEMICAL_PRINTLN("  Nc  = " << std::setw(3) << Nc << " # complementaries");
            Y_CHEMICAL_PRINTLN("  MS  = " << std::setw(3) << MS << " # spectators");
            
            assert(N+Nc==M);
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
                
            }


            Y_CHEMICAL_PRINTLN("<Reactor/>");

        }


        void Reactor:: loadK(const double t)
        {
            for(const ENode *en = eqs->head();en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                const size_t       i  = eq.indx;
                K[i] = eq.K(t);
            }
        }

        void Reactor:: computeGamma(const Accessible &C) throw()
        {
            for(const ENode *en = eqs->head();en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                const size_t       i  = eq.indx;
                Gamma[i] = eq.Gamma(C,K[i]);
            }
        }

        void Reactor:: computeGammaAndJ(const Accessible &C) throw()
        {
            J.ld(0);
            for(const ENode *en = eqs->head();en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                const size_t       i  = eq.indx;
                const double       Ki = K[i];
                Gamma[i] = eq.Gamma(C,Ki);
                eq.dGamma(J[i],C,Ki);
            }
        }



    }
    
}

