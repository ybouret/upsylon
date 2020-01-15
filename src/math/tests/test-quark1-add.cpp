
#include "y/math/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    template <typename ARRAY>
    static inline void check1D( const ARRAY &arr, const ARRAY &brr )
    {
        //std::cerr << "\tCheck " << typeid(ARRAY).name() << std::endl;
        for(size_t i=arr.size();i>0;--i)
        {
            Y_ASSERT( __mod2( arr[i] - brr[i] ) <= 0 );
        }
    }

#define __FILL() \
support::fill1D(u);\
support::fill1D(v)

#define __OPV1(NAME) do {\
support::reset1D(t); quark::NAME(t,u,v);\
if(loop) { support::reset1D(tb); quark::NAME(tb,u,v,*loop); check1D(t,tb); }\
} while(false)

#define __OPV2(NAME) do {\
support::reset1D(t); quark::NAME(t,u);\
if(loop) { support::reset1D(tb); quark::NAME(tb,u,*loop); check1D(t,tb); }\
} while(false)

#define __PROC(NAME) do {\
__FILL(); __OPV1(NAME); \
__FILL(); __OPV2(NAME); \
} while(false)

    template <typename T,
    typename U,
    typename V>
    void doOPS(concurrent::for_each *loop)
    {
        const T zt = 0;
        const U zu = 0;
        const V zv = 0;
        for(size_t iter=0;iter<8;++iter)
        {
            const size_t n = 10 + alea.leq(1000);
            vector<T> t(n,zt);
            vector<T> tb(n,zt);
            vector<U> u(n,zu);
            vector<V> v(n,zv);

            __PROC(add);
            __PROC(sub);
            __PROC(subp);

        }


    }

}

Y_UTEST(quark1_add)
{
    concurrent::simd loop;
    doOPS<float,float,float>( &loop );
    doOPS<float,int,float>( &loop );
    doOPS<mpz,int,mpz>(NULL);

}
Y_UTEST_DONE()

