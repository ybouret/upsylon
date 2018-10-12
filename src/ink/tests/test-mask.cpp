#include "y/ink/mask.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sort/unique.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename HF>
    static inline void test_mv(const vector<coord> &q,
                               vector<size_t>      &h)
    {

        rt_clock rtc;
        const size_t n = q.size();
        h.free();
        MetaVertex::Hasher<HF> H;
        std::cerr << "-- Using " << H.hfn.name() << std::endl;
        uint64_t tmx = 0;
        for(size_t i=n;i>0;--i)
        {
            const coord    p    = q[i];
            const uint64_t mark = rt_clock::ticks();
            const size_t   hk   = H(p);
            tmx += rt_clock::ticks() - mark;
            h.push_back( hk );
        }
        assert(h.size()==n);
        const double speed = (n / rtc(tmx))/1e6;
        std::cerr << "speed=" << speed << " Mops" << std::endl;
        unique(h);
        const size_t m           = h.size();
        const size_t collisions = n-m;
        std::cerr << "collisions : " << collisions << " / " << n << std::endl;
    }
}

Y_UTEST(mask)
{
    const unit_t   n = 256;
    const unit_t   n2 = n*n;
    vector<coord>  q(n2,as_capacity);
    vector<size_t> h(n2,as_capacity);

    for(size_t w=0;w<n;++w)
    {
        for(size_t h=0;h<n;++h)
        {
            q.push_back_( coord(w,h) );
        }
    }

    test_mv<hashing::sfh>(q,h);
    test_mv<hashing::sha1>(q,h);

}
Y_UTEST_DONE()

