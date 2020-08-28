

#include "y/jive/pattern/logic/ooo.hpp"
#include "y/jive/pattern/basic/rework.hpp"

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
            swap_with(strongList);
            merge_back(feebleList);
        }
    }

}

