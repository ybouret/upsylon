
#include "y/concurrent/task/contract.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/collapse.hpp"

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
        next(0), prev(0), uuid(U), plan(J)
        {
            
        }

        contract * contract:: zcreate()
        {
            return object::acquire1<contract>();
        }

        void contract:: zdelete(contract *c) throw()
        {
            assert(c);
            object::release1(c);
        }

        contract * contract:: revoked(contract *c) throw()
        {
            assert(c);
            return collapsed(c);
        }

        void contract:: release(contract *c) throw()
        {
            assert(c);
            zdelete( revoked(c) );
        }

        
    }
}
