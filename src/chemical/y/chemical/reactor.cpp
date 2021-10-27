
#include "y/chemical/reactor.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/kernel/lu.hpp"
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
        active(M,false),
        K(N,0),
        Gamma(N,0),
        J(Nu.rows,Nu.cols),
        JNuT(N,N),
        xi(N,0),
        startC(M,0),
        deltaC(M,0),
        trialC(M,0),
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

            for(const SNode *node=lib->head();node;node=node->next )
            {
                const Species &sp = ***node;
                if(sp.rating>0)
                {
                    aliasing::_(active[sp.indx]) = true;
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

        void Reactor:: computeJ(const Accessible &C) throw()
        {
            J.ld(0);
            for(const ENode *en = eqs->head();en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                const size_t       i  = eq.indx;
                const double       Ki = K[i];
                eq.dGamma(J[i],C,Ki);
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

        bool Reactor:: computeImpulse(const Accessible &C) throw()
        {
            computeGammaAndJ(C);
            tao::mmul(JNuT,J,NuT);
            return LU::build(JNuT);
        }

        bool Reactor:: computeFullStep() throw()
        {
            if(computeImpulse(startC))
            {
                tao::neg(xi,Gamma);
                LU::solve(JNuT,xi);
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "J=" << J << std::endl;
                std::cerr << "xi=" << xi << std::endl;
                tao::mul(deltaC,NuT,xi);
                std::cerr << "dC=" << deltaC << std::endl;
                std::cerr << "C =" << startC << std::endl;
                return true;
            }
            else
            {
                std::cerr << "singular C=" << startC << std::endl;
                return true;
            }
        }


        bool Reactor:: solve(Addressable &C)
        {
            if(MW<=0) return true;

            // C -> startC
            for(size_t j=M;j>0;--j)
            {
                startC[j] = C[j]; assert( !(active[j] && C[j]<=0) );
            }

        STEP:
            if(!computeFullStep())
            {
                // singular composition
                return false;
            }
            else
            {
                if( tao::mod2<double>::of(deltaC) <= 0)
                {
                    // success!
                    std::cerr << "null step :)" << std::endl;
                    return true;
                }
                // first step found
                double factor = 1.0;

            EVAL:
                std::cerr << "factor=" << factor << std::endl;
                {
                    double stp2 = 0;
                    for(size_t j=M;j>0;--j)
                    {
                        const double stp = factor * deltaC[j];
                        stp2 += stp*stp;
                        trialC[j] = startC[j] + stp;
                    }

                    if(stp2<=0)
                    {
                        std::cerr << "stuck" << std::endl;
                        return false;
                    }

                    for(size_t j=M;j>0;--j)
                    {
                        if(active[j] && trialC[j]<0)
                        {
                            factor *= 0.5;
                            goto EVAL;
                        }
                    }
                }


                bool converged = true;
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        const double dC = fabs(trialC[j]-startC[j]);
                        if( dC > fabs(startC[j]) * numeric<double>::sqrt_ftol )
                        {
                            converged = false;
                        }
                        startC[j] = trialC[j];
                    }
                }

                if(!converged)
                    goto STEP;

                std::cerr << "converged" << std::endl;
                return true;
            }
        }

        bool Reactor:: damp(Addressable &rate, const Addressable &C)
        {
            computeJ(C);
            std::cerr << "J=" << J << std::endl;
            tao::mmul(JNuT,J,NuT);
            if(!LU::build(JNuT))
            {
                return false;
            }
            else
            {
                tao::mul(xi,J,rate);
                LU::solve(JNuT,xi);
                tao::mul(deltaC,NuT,xi);
                tao::sub(rate,deltaC);
                return true;
            }


        }

    }
    
}

