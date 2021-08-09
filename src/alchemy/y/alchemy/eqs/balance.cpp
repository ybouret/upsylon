#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {


        

        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    showCondition(std::cerr,sp,C);
                }
                std::cerr << "Primary Conditions: " << std::endl;
                for(size_t i=Cond.size();i>0;--i)
                {
                    Cond[i].show(std::cerr,C) << std::endl;
                }
                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


