#include "y/ink/mask.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/fnv.hpp"
#include "y/core/locate.hpp"
#include "y/string/convert.hpp"
#include "y/associative/map.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename HF>
    static inline void test_mv(const vector<coord> &q)
    {

        rt_clock rtc;
        const size_t n = q.size();

        MetaVertex::Hasher<HF> H;
        std::cerr << "-- Using " << H.hfn.name() << std::endl;
        map<size_t,size_t> h(n,as_capacity);

        uint64_t tmx  = 0;
        size_t   coll = 0;
        for(size_t i=n;i>0;--i)
        {
            const coord    p    = q[i];
            const uint64_t mark = rt_clock::ticks();
            const size_t   hk   = H(p);
            tmx += rt_clock::ticks() - mark;
            //core::insert_sorted(hk,h,comparison::increasing<size_t>);
            if(!h.insert(hk,hk))
            {
                ++coll;
            }
        }
        const double speed = (n/rtc(tmx))/1e6;
        std::cerr << "speed=" << speed << " Mops" << std::endl;
        std::cerr << "#coll=" << coll << "/" << n << std::endl;
    }
}

Y_UTEST(mask)
{

    unit_t   n = 128;
    if( argc > 1 )
    {
        n = string_convert::to<unit_t>(argv[1],"n");
    }
    const unit_t   n2 = n*n;
    vector<coord>  q(n2,as_capacity);

    for(unit_t w=0;w<n;++w)
    {
        for(unit_t h=0;h<n;++h)
        {
            q.push_back_( coord(w,h) );
        }
    }

    test_mv<hashing::sfh>(q);
    test_mv<hashing::sha1>(q);
    test_mv<hashing::fnv>(q);

}
Y_UTEST_DONE()

