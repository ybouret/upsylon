
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
        W(),
        arrN(10),
        xi( arrN.next() ),
        arrM(10),
        Corg( arrM.next() ),
        Caux( arrM.next() ),
        Ctry( arrM.next() ),
        Cstp( arrM.next() ),
        Cusr( arrM.next() ),
        used(),
        balanceVerbose(false)
        {
        }

        void Solver:: quit() throw()
        {
            new (&used) Booleans();

            arrM. release();
            arrN. release();
            W.    release();
            Nu2.  release();
            tNu.  release();
            Nu.   release();
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
                    Nu.   make(N,M);
                    tNu.  make(M,N);
                    Nu2.  make(M,M);
                    W.    make(N,N);
                    arrN. acquire(N);
                    
                    eqs.fillNu(Nu);
                    tNu.assign_transpose(Nu);
                    quark::mmul(Nu2, tNu, Nu);
                }

                if(M>0)
                {
                    arrM.acquire(M);
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
