
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
        W(),
        aN(3),
        xi(  aN.next()  ),
        nu2( aN.next()  ),
        aM(5),
        Corg( aM.next() ),
        Caux( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        tmp_( aM.next() ),
        active(),
        clr(),
        balanceVerbose(false)
        {
            clr << Nu;
            clr << tNu;
            clr << Nu2;
            clr << W;
            clr << aN;
            clr << aM;
        }

        void Solver:: quit() throw()
        {
            new ( &aliasing::_(active) ) Booleans();
            clr.release_all();
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
                    new (&act) Booleans( aliasing::as<bool,double>(*tmp_), M );
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


    }

}
