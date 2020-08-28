

#include "y/jive/pattern/logic/ooo.hpp"

namespace upsylon {

    namespace Jive
    {

        OutOfOrder:: ~OutOfOrder() throw()
        {
        }

        OutOfOrder:: OutOfOrder(const uint32_t t) throw() :
        Logical(t)
        {
        }

        OutOfOrder:: OutOfOrder(const OutOfOrder &other) :
        Logical(other)
        {
        }

    }

}

#include "y/jive/pattern/basic/rework.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace Jive
    {

#if 0
        static int byIncreasingEntropy(const Pattern *lhs, const Pattern * rhs, void *) throw()
        {
            const double L = lhs->entropy();
            const double R = rhs->entropy();
            return comparison::increasing(L,R);
        }
#endif
        void OutOfOrder:: rework() throw()
        {
            Operands strongList;
            Operands feebleList;
            while(size)
            {
                if(head->feeble())
                {
                    feebleList.push_back( pop_front() );
                }
                else
                {
                    strongList.push_back( pop_front() );
                }
            }

            Rework::Compact(strongList);
            //merging<Pattern>::sort(strongList,byIncreasingEntropy,NULL);
            swap_with(strongList);
            merge_back(feebleList);
        }
    }

}

