
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
        equilibria(),
        Nu(),
        tNu(),
        Nu2(),
        det(0),
        Phi(),
        W(),
        aN(4),
        K(   aN.next()  ),
        Q(   aN.next()  ),
        xi(  aN.next()  ),
        nu2( aN.next()  ),
        aM(8),
        Corg( aM.next() ),
        Caux( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        Cini( aM.next() ),
        Cend( aM.next() ),
        Cswp( aM.next() ),
        active(),
        extra( aM.next() ),
        keeper(),
        balanceVerbose(false),
        lastBalanceCycles(0),
        forwardVerbose(false),
        lastForwardCycles(0)
        {
            keeper << aliasing::_(equilibria);
            keeper << aliasing::_(Nu);
            keeper << aliasing::_(tNu);
            keeper << aliasing::_(Nu2);
            keeper << Phi;
            keeper << W;
            keeper << aN;
            keeper << aM;
        }

        void Solver:: quit() throw()
        {
            new ( &aliasing::_(active) ) Booleans();
            keeper.release_all();
            aliasing::_(det)  = 0;
            aliasing::_(P)    = 0;
            aliasing::_(A)    = 0;
            aliasing::_(M)    = 0;
            aliasing::_(N)    = 0;
        }


        void Solver:: init(Library &lib, const Equilibria &eqs)
        {
            static const char fn[] = "Aqua::Solver: ";
            quit();
            try
            {
                //--------------------------------------------------------------
                // prepare counts
                //--------------------------------------------------------------
                lib.buildIndices();
                aliasing::_(N) = eqs.entries();
                aliasing::_(M) = lib.entries();
                if(N>M) throw exception("%stoo many equilibria",fn);
                aliasing::_(P) = M-N;

                //--------------------------------------------------------------
                // load eqs
                //--------------------------------------------------------------
                assert(equilibria.size()<=0);
                EqVector &EQS = aliasing::_(equilibria);
                EQS.ensure(N);
                {
                    size_t i=1;
                    for(Equilibria::const_iterator it=eqs.begin();i<=N;++i,++it)
                    {
                        const Equilibrium::Pointer &eq = *it;
                        eq->validate();
                        EQS.push_back_(eq);
                    }
                }


                if(N>0)
                {
                    aliasing::_(Nu).  make(N,M);
                    aliasing::_(tNu). make(M,N);
                    aliasing::_(Nu2).  make(N,N);
                    Phi.   make(N,M);
                    W.     make(N,N);
                    aN.    acquire(N);

                    //----------------------------------------------------------
                    // build topology
                    //----------------------------------------------------------
                    for(size_t i=N;i>0;--i)
                    {
                        equilibria[i]->fillNu(aliasing::_(Nu[i]));
                    }

                    //----------------------------------------------------------
                    // build transposed and Gram's matrix
                    //----------------------------------------------------------
                    aliasing::_(tNu).assign_transpose(Nu);
                    quark::mmul(aliasing::_(Nu2),Nu,tNu);

                    //----------------------------------------------------------
                    // check independant matrices
                    //----------------------------------------------------------
                    aliasing::_(det) = ideterminant(Nu2);
                    if(0==det)
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

        void Solver:: computeK(const double t)
        {
            assert(N==equilibria.size());
            for(size_t i=N;i>0;--i)
            {
                K[i] = equilibria[i]->K(t);
            }
        }

        void Solver:: computeS(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(Phi[i], K[i], C);
            }
        }

        bool Solver:: computeW() throw()
        {
            quark::mmul_rtrn(W,Phi,Nu);
            return LU::build(W);
        }

        void Solver:: computeQ(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(K[i],C);
            }
        }

    }

}
