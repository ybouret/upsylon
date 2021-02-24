
#include "y/concurrent/task/deed.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        deed:: ~deed() throw()
        {
            aliasing::_(uuid) = 0;
        }
        
        deed:: deed(const job::uuid U, const job::type &J) :
        next(0), prev(0), uuid(U), func(J)
        {
            
        }

        
    }
}
