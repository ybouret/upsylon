#include "y/math/kernel/atom.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "support.hpp"
#include <cstring>
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    struct LD
    {

        template <typename ARRAY> static inline
        void test( ARRAY &arr, concurrent::for_each &loop)
        {
            typedef typename ARRAY::mutable_type type;
            const type   value = support::get<type>();
            support::fill1D(arr);
            Y_SUPPORT_TICKS(fullTicks,atom::ld(arr,value));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,value) <= 0 );
            support::fill1D(arr);
            Y_SUPPORT_TICKS(loopTicks,atom::ld(arr,value,loop));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,value) <= 0 );
            rt_clock clk;
            std::cerr << clk.speedup(fullTicks,loopTicks) << '/';
        }

        template <typename T> static inline
        void testAll(concurrent::for_each &loop)
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter LD for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";
                { vector<T,memory::global> gv(nn); test(gv,loop); }
                { vector<T,memory::pooled> pv(nn); test(pv,loop); }
                { list<T>                  gl(nn); test(gl,loop); }
                if(2==nn)
                {
                    point2d<T> p; test(p,loop);
                }
                if(3==nn)
                {
                    point3d<T> p; test(p,loop);
                }
                std::cerr << std::endl;

            }
            std::cerr << "[Leave LD for <" << tid << ">]" << std::endl;

        }
    };

    struct NEG
    {

        template <typename ARRAY> static inline
        void test( ARRAY &arr, concurrent::for_each &loop)
        {
            typedef typename ARRAY::mutable_type type;
            const type   value = support::get<type>();
            const type   minus = -value;
            atom::ld(arr,value);
            Y_SUPPORT_TICKS(fullTicks,atom::neg(arr));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,minus) <= 0 );

            atom::ld(arr,value);
            Y_SUPPORT_TICKS(loopTicks,atom::neg(arr,loop));
            Y_ASSERT( atom::tool::deltaSquaredTo(arr,minus) <= 0 );
            rt_clock clk;
            std::cerr << clk.speedup(fullTicks,loopTicks) << '/';
        }

        template <typename T> static inline
        void testAll(concurrent::for_each &loop)
        {
            static const size_t n[] = { 1,2,3,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
            static const char  *tid = typeid(T).name();

            std::cerr << "[Enter NEG for <" << tid << ">]" << std::endl;
            for(size_t i=0;i<sizeof(n)/sizeof(n[0]);++i)
            {
                const size_t nn = n[i];
                std::cerr << "\t" << nn << ":";
                { vector<T,memory::global> gv(nn); test(gv,loop); }
                { vector<T,memory::pooled> pv(nn); test(pv,loop); }
                { list<T>                  gl(nn); test(gl,loop); }
                if(2==nn)
                {
                    point2d<T> p; test(p,loop);
                }
                if(3==nn)
                {
                    point3d<T> p; test(p,loop);
                }
                std::cerr << std::endl;

            }
            std::cerr << "[Leave NEG for <" << tid << ">]" << std::endl;
        }
    };

    struct Level1
    {
        template <typename T> static inline
        void Test( concurrent::for_each &loop )
        {
            LD::testAll<T>(loop);
            NEG::testAll<T>(loop);
        }


    };


}

Y_UTEST(atom1)
{
    concurrent::simd loop;

    Level1::Test<float>(loop);
    Level1::Test<double>(loop);
    Level1::Test<short>(loop);

}
Y_UTEST_DONE()

