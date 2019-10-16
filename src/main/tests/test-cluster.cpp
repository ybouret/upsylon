#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/ptr/batch.hpp"
#include "y/memory/pooled.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(cluster)
{

    batch_ptr< cluster<int>, shared_ptr> L = new list<int>();
    batch_ptr< cluster<int>, shared_ptr> G = new vector<int,memory::global>();
    batch_ptr< cluster<int>, shared_ptr> P = new vector<int,memory::pooled>();

}
Y_UTEST_DONE()



