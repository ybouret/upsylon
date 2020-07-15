
#include "y/counting/permuter.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    void doTestWith( concurrent::for_each &proc )
    {
        std::cerr << "#threads=" << proc.number() << std::endl;
    }

}


Y_UTEST(perm_par)
{
    concurrent::sequential_for seq;
    concurrent::simd           par;

    doTestWith(seq);
    doTestWith(par);
    
}
Y_UTEST_DONE()

