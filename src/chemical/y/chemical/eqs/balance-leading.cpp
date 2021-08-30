#include "y/chemical/reactor.hpp"

namespace upsylon
{

    namespace Chemical
    {
        bool Reactor:: balanceLeading(Addressable &C) const throw()
        {
            Vector xi(N,0);

            for(size_t i=N;i>0;--i)
            {
               const Leading::Status status =  leading[i]->solve(C, NuT, xi);
            }
            return false;
        }


    }

}

