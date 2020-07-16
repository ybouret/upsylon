
#include "y/counting/permuter.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/string.hpp"
#include "y/sequence/vector.hpp"

#include <iomanip>

using namespace upsylon;

namespace {

    

}


Y_UTEST(perm_par)
{
    concurrent::sequential_for seq;
    concurrent::simd           par;



}
Y_UTEST_DONE()

