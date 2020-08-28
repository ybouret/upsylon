

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

        void OutOfOrder:: append(Pattern *p) throw()
        {
            assert(p);
            operands.push_back(p);
            if(p->strong())
            {
                while(p->prev&&p->prev->feeble())
                {
                    operands.move_to_front(p);
                }
            }
        }

#if 1
        void OutOfOrder:: harden() throw()
        {
            Operands s,f;
            while(operands.size)
            {
                if(operands.head->feeble())
                {
                    f.push_back( operands.pop_front() );
                }
                else
                {
                    s.push_back( operands.pop_front() );
                }
            }

            operands.swap_with(s);
            operands.merge_back(f);
        }
#endif
    }

}

