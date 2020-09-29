
#include "y/aqua/solver.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/kernel/quark.hpp"

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
        Nu(),
        tNu(),
        Nu2(),
        W(),
        aN(2),
        B(  aN.next() ),
        xi( aN.next() ),
        aM(5),
        Corg( aM.next() ),
        Caux( aM.next() ),
        Ctry( aM.next() ),
        Cstp( aM.next() ),
        Cusr( aM.next() ),
        used(),
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
            new (&used) Booleans();
            clr.release_all();
            aliasing::_(M) = 0;
            aliasing::_(N) = 0;
        }


        void Solver:: init(Library &lib, Equilibria &eqs)
        {
            quit();
            try
            {
                lib.buildIndices();
                eqs.validate();

                aliasing::_(N) = eqs.entries();
                aliasing::_(M) = lib.entries();

                if(N>0)
                {
                    Nu.    make(N,M);
                    tNu.   make(M,N);
                    Nu2.   make(M,M);
                    W.     make(N,N);
                    aN.    acquire(N);
                    eqs.fillNu(Nu);
                    tNu.assign_transpose(Nu);
                    quark::mmul(Nu2, tNu, Nu);

                }

                if(M>0)
                {
                    aM.acquire(M);
                    new (&used) Booleans( aliasing::as<bool,double>(*Cusr), M );
                    quark::ld(used,false);
                    for(size_t i=N;i>0;--i)
                    {
                        const accessible<int> &nu_i = Nu[i];
                        for(size_t j=M;j>0;--j)
                        {
                            if( nu_i[j] != 0) used[j] = true;
                        }
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
