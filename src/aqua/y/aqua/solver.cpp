
#include "y/aqua/solver.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/determinant.hpp"
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
        det(0),
        Nu(),
        tNu(),
        Nu2(),
        R(),
        W(),
        aN(3),
        B(   aN.next()  ),
        xi(  aN.next()  ),
        nu2( aN.next()  ),
        aM(5),
        Corg( aM.next() ),
        Caux( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        tmp_( aM.next() ),
        used(),
        clr(),
        balanceVerbose(false)
        {
            clr << Nu;
            clr << tNu;
            clr << Nu2;
            clr << R;
            clr << W;
            clr << aN;
            clr << aM;
        }

        void Solver:: quit() throw()
        {
            new (&used) Booleans();
            clr.release_all();
            aliasing::_(det) = 0;
            aliasing::_(P)   = 0;
            aliasing::_(A)   = 0;
            aliasing::_(M)   = 0;
            aliasing::_(N)   = 0;
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
                    aliasing::_(det) = ideterminant(Nu2);
                    if(0==det)
                    {
                        throw exception("Aqua::Solver(singular equilibria)");
                    }
                    for(size_t i=N;i>0;--i)
                    {
                        int sum = 0;
                        const accessible<int>  &Nu_i = Nu[i];
                        for(size_t j=M;j>0;--j) sum += square_of(Nu_i[j]);
                        nu2[i] = double(sum);
                    }
                }

                if(M>0)
                {
                    aM.acquire(M);
                    new (&used) Booleans( aliasing::as<bool,double>(*tmp_), M );
                    quark::ld(used,false);
                    for(size_t i=N;i>0;--i)
                    {
                        const accessible<int> &nu_i = Nu[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if( nu_i[j] != 0) used[j] = true;
                        }
                    }
                    for(size_t j=M;j>0;--j)
                    {
                        if(used[j]) ++aliasing::_(A);
                    }
                }

                if(P>0)
                {
                    R.make(P,M);
                    iMatrix I(M,M);
                    for(size_t i=N;i>0;--i)
                    {
                        quark::set(I[i],Nu[i]);
                    }
                    for(size_t i=M;i>N;--i)
                    {
                        I[i][i] = 1;
                    }
                    if(!GramSchmidt::iOrtho(I))
                    {
                        throw exception("%scannot find orthogonal basis",fn);
                    }
                    for(size_t i=P;i>0;--i)
                    {
                        quark::set(R[i],I[N+i]);
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
