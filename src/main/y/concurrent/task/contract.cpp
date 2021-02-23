#include "y/concurrent/task/contract.hpp"

namespace upsylon
{
    namespace concurrent
    {

        contract:: ~contract() throw()
        {
            assert(0==next);
            assert(0==prev);
        }


        contract:: contract(const job::uuid U, const job::type &J) :
        next(0), prev(0), uuid(U), type(J)
        {
        }

    }

}
