

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

        void OutOfOrder:: append(Pattern *p) throw()
        {
            assert(p);
            operands.push_back(p);
            harden();
        }

        void OutOfOrder:: harden() throw()
        {
            Operands strongList;
            Operands feebleList;
            while(operands.size)
            {
                if(operands.head->feeble())
                {
                    feebleList.push_back( operands.pop_front() );
                }
                else
                {
                    strongList.push_back( operands.pop_front() );
                }
            }

            Rework::Compact(strongList);
            operands.swap_with(strongList);
            operands.merge_back(feebleList);
        }
    }

}

