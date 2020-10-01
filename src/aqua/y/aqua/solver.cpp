
#include "y/aqua/solver.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        Solver:: ~Solver() throw()
        {
        }

        Solver:: Solver() :
        N(0),
        M(0),
        A(0),
        P(0),
        dNu2(0),
        Nu(),
        tNu(),
        Nu2(),
        Phi(),
        W(),
        aN(4),
        K(   aN.next()  ),
        Q(   aN.next()  ),
        xi(  aN.next()  ),
        nu2( aN.next()  ),
        aM(6),
        Corg( aM.next() ),
        Caux( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        Cfwd( aM.next() ),
        active(),
        extra( aM.next() ),
        keeper(),
        balanceVerbose(false)
        {
            keeper << Nu;
            keeper << tNu;
            keeper << Nu2;
            keeper << Phi;
            keeper << W;
            keeper << aN;
            keeper << aM;
        }

        void Solver:: quit() throw()
        {
            new ( &aliasing::_(active) ) Booleans();
            keeper.release_all();
            aliasing::_(dNu2) = 0;
            aliasing::_(P)    = 0;
            aliasing::_(A)    = 0;
            aliasing::_(M)    = 0;
            aliasing::_(N)    = 0;
        }


        void Solver:: init(Library &lib, Equilibria &eqs)
        {
            static const char fn[] = "Aqua::Solver: ";
            quit();
            try
            {
                lib.buildIndices();
                eqs.validate();

                aliasing::_(N) = eqs.entries();
                aliasing::_(M) = lib.entries();
                if(N>M) throw exception("%stoo many equilibria",fn);
                aliasing::_(P) = M-N;

                if(N>0)
                {
                    Nu.    make(N,M);
                    tNu.   make(M,N);
                    Nu2.   make(N,N);
                    Phi.   make(N,M);
                    W.     make(N,N);
                    aN.    acquire(N);
                    eqs.fillNu(Nu);
                    tNu.assign_transpose(Nu);
                    quark::mmul(Nu2,Nu,tNu);
                    aliasing::_(dNu2) = ideterminant(Nu2);
                    if(0==dNu2)
                    {
                        throw exception("Aqua::Solver(singular equilibria)");
                    }

                    for(size_t i=N;i>0;--i)
                    {
                        double sum = 0;
                        for(size_t j=M;j>0;--j)
                        {
                            sum += square_of(Nu[i][j]);
                        }
                        nu2[i] = sum;
                    }

                }

                if(M>0)
                {
                    aM.acquire(M);
                    Booleans &act = aliasing::_(active);
                    new (&act) Booleans( aliasing::as<bool,double>(*extra), M );
                    quark::ld(act,false);
                    for(size_t i=N;i>0;--i)
                    {
                        const accessible<int> &nu_i = Nu[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if( nu_i[j] != 0) act[j] = true;
                        }
                    }
                    assert(A<=0);
                    for(size_t j=M;j>0;--j)
                    {
                        if(act[j]) ++aliasing::_(A);
                    }
                }                

            }
            catch(...)
            {
                quit();
                throw;
            }

        }

        void Solver:: computeK(const Equilibria &eqs, const double t)
        {
            eqs.computeK(K,t);
        }

        void Solver:: computePhi(const Equilibria         &eqs,
                                 const accessible<double> &C) throw()
        {
            eqs.computePhi(Phi,K,C);
        }

        bool Solver:: computeW() throw()
        {
            quark::mmul_rtrn(W,Phi,Nu);
            return LU::build(W);
        }

        void Solver:: computeQ(const Equilibria &eqs, const accessible<double> &C) throw()
        {
            eqs.computeQ(Q,K,C);
        }

    }

}
