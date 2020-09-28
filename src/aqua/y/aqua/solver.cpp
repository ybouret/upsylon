
#include "y/aqua/solver.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/kernel/quark.hpp"

namespace upsylon
{

    namespace Aqua
    {
        Solver:: ~Solver() throw()
        {
        }

        Solver:: Solver() :
        N(0),
        M(0),
        nu(),
        arr(10),
        Corg( arr.next() ),
        Caux( arr.next() ),
        Ctry( arr.next() ),
        Cstp( arr.next() ),
        Cusr( arr.next() ),
        used()
        {
        }

        void Solver:: quit() throw()
        {
            new (&used) Booleans();
            arr. release();
            nu.  release();
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
                    nu.make(N,M);
                    eqs.fillNu(nu);
                }

                if(M>0)
                {
                    arr.acquire(M);
                    new (&used) Booleans( aliasing::as<bool,double>(*Cusr), M );
                    mkl::quark::ld(used,false);
                    for(size_t i=N;i>0;--i)
                    {
                        const accessible<double> &nu_i = nu[i];
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
