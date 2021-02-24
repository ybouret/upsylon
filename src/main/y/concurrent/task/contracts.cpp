
#include "y/concurrent/task/contracts.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace concurrent
    {
        
        contracts:: contracts() throw() : pending(), shallow()
        {
        }
        
        contracts:: ~contracts() throw()
        {
            release();
        }

        void contracts:: release() throw()
        {
            remove_shallow();
            remove_pending();
        }

        void contracts:: delete_pending(contract *c) throw()
        {
            assert(c);
            self_destruct(*c);
            object::release1(c);
        }

        void contracts:: delete_shallow(contract *z) throw()
        {
            assert(z);
            object::release1(z);
        }

        void contracts:: remove_shallow() throw()
        {
            shallow.yield(delete_shallow);
        }

        void contracts:: remove_pending() throw()
        {
            pending.yield(delete_pending);
        }

        
        void contracts:: terminate(contract * &c) throw()
        {
            assert(c);
            self_destruct(*c);
            shallow.push_back(c);
            c = 0;
        }


        
        void contracts:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                shallow.push_back( object::acquire1<contract>() );
            }
        }
        
        void contracts:: establish(const job::uuid U, const job::type &J)
        {
            contract *c = (shallow.size<0) ? shallow.pop_back() : object::acquire1<contract>();
            try
            {
                pending.push_back( new (c) contract(U,J) );
            }
            catch(...)
            {
                shallow.push_back(c);
                throw;
            }
        }

        
        
    }
    
}
