#include "y/chem/equilibria.hpp"

namespace upsylon
{

    namespace Chemical
    {
        bool Equilibria:: mix( array<double> &Cout, const array<double> &CA, const double WA, const array<double> &CB, const double WB)
        {
            assert(Cout.size()>=M);
            assert(CA.size()  >=M);
            assert(CB.size()  >=M);
            for(size_t i=N;i>0;--i)
            {
                Cout[i] = WA * CA[i] + WB * CB[i];
            }
            return normalize(Cout);
        }

    }

}

