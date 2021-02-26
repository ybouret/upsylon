
#include "y/concurrent/task/contracts.hpp"

namespace upsylon
{
    namespace concurrent
    {
        contracts:: ~contracts() throw()
        {
            assert(0==size);
        }

        contracts:: contracts() throw()
        {}

        
    }

}

namespace upsylon
{
    namespace concurrent
    {
        pending:: pending() throw()
        {
        }

        pending:: ~pending() throw()
        {
            release();
        }

        void pending:: release() throw()
        {
            yield(contract::release);
        }

        void pending:: append(const job::uuid U, const job::type &J, settled &pool)
        {
            contract *c = pool.size ? pool.pop_back() : contract::zcreate();
            try
            {
                push_back( new (c) contract(U,J) );
            }
            catch(...)
            {
                pool.push_back(c);
                throw;
            }
        }
    }
}




namespace upsylon
{
    namespace concurrent
    {
        settled:: settled() throw() : contracts()
        {
        }
        
        settled:: ~settled() throw()
        {
            release();
        }
        
        void settled:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                push_back( contract::zcreate() );
            }
        }
        
        void settled:: release() throw()
        {
            yield(contract::zdelete);
        }

        
        void settled:: cancel(contract *alive) throw()
        {
            assert(alive);
            push_back( contract::revoked(alive) );
        }

        
        void settled:: cancel(pending &source) throw()
        {
            while(source.size)
            {
                cancel( source.pop_back() );
            }
            
        }

    }
    
}

