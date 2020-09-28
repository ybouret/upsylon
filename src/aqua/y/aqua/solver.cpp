
#include "y/aqua/solver.hpp"
#include "y/type/aliasing.hpp"

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
        Ctry( arr.next() )
        {
        }

        void Solver:: quit() throw()
        {
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

                

            }
            catch(...)
            {
                quit();
                throw;
            }

        }


    }

}
