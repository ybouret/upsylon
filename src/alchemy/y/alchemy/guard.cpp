
#include "y/alchemy/guard.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        Guard:: ~Guard() throw()
        {
        }

        Guard:: Guard() throw() :
        leq(), geq(), cls(HasNoBound)
        {
        }

        Guard:: Array:: ~Array() throw()
        {
        }

        Guard::Array:: Array(size_t n) :
        Array_(n,as_capacity)
        {
            while(n-- > 0)
            {
                const Guard::Pointer p = new Guard();
                push_back_(p);
            }
        }


    }

}
