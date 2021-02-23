
#include "y/concurrent/task/contracts.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace concurrent
    {
        
        contracts:: contracts() throw() : pending(), zombies()
        {
        }
        
        contracts:: ~contracts() throw()
        {
            while(pending.size)
            {
                contract *c = pending.pop_back();
                self_destruct(*c);
                object::release1(c);
            }
            trim();
        }
        
        
        void contracts:: keep(const size_t n) throw()
        {
            while(zombies.size>n)
            {
                contract *z = zombies.pop_back();
                object::release1(z);
            }
        }
        
        void contracts:: trim() throw()
        {
            keep(0);
        }
        
        void contracts:: hire(size_t n)
        {
            while(n-- > 0)
            {
                zombies.push_back( object::acquire1<contract>() );
            }
        }
        
        void contracts:: establish(const job::uuid U, const job::type &J)
        {
            contract *c = zombies.size ? zombies.pop_back() : object::acquire1<contract>();
            try
            {
                pending.push_back( new (c) contract(U,J) );
            }
            catch(...)
            {
                zombies.push_back(c);
                throw;
            }
        }
        
        
    }
    
}
