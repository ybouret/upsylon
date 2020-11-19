
#include "y/associative/hash/slots.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{


    static inline
    void doHashSlots(const size_t n)
    {
        static const size_t one = 1;
        hash_slots          hs(n);
        const size_t required  = hs.slots * sizeof(hash_meta_list);
        const size_t allocated = one << hs.sexp2;
        std::cerr << "n=" << n << std::endl;
        std::cerr << "\tslots     = " << hs.slots << std::endl;
        std::cerr << "\tsmask     = " << hs.smask << std::endl;
        std::cerr << "\tsexp2     = " << hs.sexp2 << std::endl;
        std::cerr << "\tbytes     = " << allocated << "/" << required << std::endl;
        Y_CHECK( allocated >= required );

        hash_meta_pool pool;
        try {
            int data = 1;

            for(size_t i=1+alea.leq(100);i>0;--i)
            {
                const size_t    hkey = alea.full<uint16_t>();
                hs[hkey].push_front( pool.query(hkey,&data) );
            }

            hs.dump();

            
            hs.to(pool);
        }
        catch(...)
        {
            hs.to(pool);
            throw;
        }

        pool.sort();
        std::cerr << "pool.size=" << pool.size << std::endl;
        std::cerr << std::endl;
    }

}

Y_UTEST(hashed)
{
    doHashSlots(0);
    for(size_t n=1;n<=16;n<<=1)
    {
        doHashSlots(n);
    }

}
Y_UTEST_DONE()


