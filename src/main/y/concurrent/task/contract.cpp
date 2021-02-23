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
            gc();
        }


        void contracts:: gc() throw()
        {
            while(zombies.size)
            {
                contract *z = zombies.pop_back();
                object::release1(z);
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
