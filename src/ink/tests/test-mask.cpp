#include "y/ink/mask.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/sfh.hpp"
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

    static inline unit_t new_unit() { return 256 - unit_t(alea.leq(512)); }
}

Y_UTEST(mask)
{
    if(false)
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

    Mask msk1;
    for(size_t iter=0;iter<10000;++iter)
    {
        msk1.append(new_unit(),new_unit());
    }
    std::cerr << "#msk1=" << msk1.size() << std::endl;

    Mask msk2 = msk1;
    std::cerr << "#msk2=" << msk2.size() << std::endl;
    msk2.free();
    for(size_t iter=0;iter<10000;++iter)
    {
        msk2.append(new_unit(),new_unit());
    }
    std::cerr << "#msk2=" << msk2.size() << std::endl;
    const Mask msk0 = msk1;

    msk1.__and(msk2);
    std::cerr << "msk1|=msk2 => " << msk1.size() << std::endl;

    msk1 = msk0;
    msk1.__or(msk2);
    std::cerr << "msk1&=msk2 => " << msk1.size() << std::endl;

    msk1 = msk0;
    msk1.__xor(msk2);
    std::cerr << "msk1^=msk2 => " << msk1.size() << std::endl;

}
Y_UTEST_DONE()

