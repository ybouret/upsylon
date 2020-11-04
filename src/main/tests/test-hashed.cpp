
#include "y/associative/hash/slots.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    struct dummy {
        dummy *next;
        dummy *prev;
    };


    static inline
    void doHashSlots(const size_t n)
    {
        static const size_t one = 1;
        hash_slots<dummy> hs(n);
        const size_t required  = hs.block_size * hs.slot_size;
        const size_t allocated = one << hs.block_exp2;
        std::cerr << "n=" << n << std::endl;
        std::cerr << "\tslot_size = " << hs.slot_size << std::endl;
        std::cerr << "\tsize      = " << hs.block_size << std::endl;
        std::cerr << "\tmask      = " << hs.block_mask << std::endl;
        std::cerr << "\texp2      = " << hs.block_exp2 << std::endl;
        std::cerr << "\tbytes     = " << allocated << "/" << required << std::endl;
        Y_CHECK( allocated >= required );
        std::cerr << std::endl;
    }

}

Y_UTEST(hashed)
{
    doHashSlots(0);
    for(size_t n=1;n<=128;n<<=1)
    {
        doHashSlots(n);
    }

}
Y_UTEST_DONE()


