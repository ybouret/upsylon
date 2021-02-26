
#include "y/concurrent/task/contract.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        contract:: ~contract() throw()
        {
            assert(0==next);
            assert(0==prev);
            aliasing::_(uuid) = 0;
        }
        
        contract:: contract(const job::uuid U, const job::type &J) :
        next(0), prev(0), uuid(U), func(J)
        {
            
        }

        
    }
}
