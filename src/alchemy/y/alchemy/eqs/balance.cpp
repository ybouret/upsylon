#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        

        void Reactor:: restrain(Addressable      &extents,
                                const Accessible &concentrations) const throw()
        {
            for(size_t i=cond.size();i>0;--i)
            {
                cond[i](extents,concentrations);
            }
        }


        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);
                
                


                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


