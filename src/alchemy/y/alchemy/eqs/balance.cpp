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
        void    Reactor:: RestrainXi(const Accessible &C) throw()
        {
            for(size_t j=Cond.size();j>0;--j)
            {
                const Condition &cond = Cond[j];
                cond( aliasing::_(xi1), C);
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


