#include "y/hashing/sha1.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(hashing)
{
    vector< hashing::function::pointer > phash;
#define __REGISTER(CLASS) do { hashing::function::pointer p = new hashing:: CLASS(); phash.push_back(p); } while(false)
    __REGISTER(sha1);
}
Y_UTEST_DONE()

