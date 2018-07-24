#include "y/math/io/data-set.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T>
    static inline
    void load(const string &filename,
              const size_t  ix,
              const size_t  iy)
    {
        data_set<T> ds;
        ios::icstream fp(filename);
        vector<T> x;
        list<T>   y;
        ds.use(ix,x);
        ds.use(iy,y);

        ds.free_all();
        ds.load(fp);
    }
}

Y_UTEST(data_set)
{

    if(argc>1)
    {
        const string fn = argv[1];
        load<float>(fn,1,2);
        load<double>(fn,2,1);
    }

}
Y_UTEST_DONE()

