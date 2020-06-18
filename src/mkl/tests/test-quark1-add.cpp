
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include "y/type/spec.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename T>
    static inline void displayBin(const T &args)
    {
        const uint8_t *p = (const uint8_t *)&args;
        std::cerr << "0x";
        for(size_t i=0;i<sizeof(T);++i)
        {
            std::cerr << hexadecimal::lowercase[p[i]];
        }
    }
    template <typename ARRAY>
    static inline void check1D( const ARRAY &arr, const ARRAY &brr )
    {
        //std::cerr << "\tCheck " << typeid(ARRAY).name() << std::endl;
        for(size_t i=arr.size();i>0;--i)
        {
            typename ARRAY::const_type d2 = __mod2( arr[i] - brr[i] );
            if( ! (d2<=0) )
            {
                std::cerr << "Mismatch #" << i  << "/" << arr.size() << " : " << arr[i] << " | " << brr[i] << std::endl;
                displayBin(arr[i]); std::cerr << " | ";   displayBin(brr[i]); std::cerr << std::endl;
                std::cerr << "d2=" << d2 << std::endl;
                throw exception("check failure");
            }
            //Y_ASSERT( __mod2( arr[i] - brr[i] ) <= 0 );
        }
    }

#define __FILL() \
support::fill1D(u);\
support::fill1D(v)

#define __OPV1(NAME) do {\
std::cerr << "|_" << #NAME << "_V1" << std::endl;\
support::reset1D(t); quark::NAME(t,u,v);\
if(loop) { support::reset1D(tb); quark::NAME(tb,u,v,*loop); check1D(t,tb); }\
} while(false)

#define __OPV2(NAME) do {\
std::cerr << "|_" << #NAME << "_V2" << std::endl;\
support::reset1D(t); quark::NAME(t,u);\
if(loop) { support::reset1D(tb); quark::NAME(tb,u,*loop); check1D(t,tb); }\
} while(false)

#define __PROC(NAME) do {\
__FILL(); __OPV1(NAME); \
__FILL(); __OPV2(NAME); \
} while(false)

#define __MPV1(NAME) do {\
std::cerr << "|_" << #NAME << "_V1" << std::endl;\
const T x = support::get<T>();\
support::reset1D(t); quark::NAME(t,u,x,v);\
if(loop) { support::reset1D(tb); quark::NAME(tb,u,x,v,*loop); check1D(t,tb); }\
} while(false)

#define __MPV2(NAME) do {\
std::cerr << "|_" << #NAME << "_V2" << std::endl;\
const T x = support::get<T>();\
support::reset1D(t); quark::NAME(t,x,u);\
if(loop) { support::reset1D(tb); quark::NAME(tb,x,u,*loop);\
check1D(t,tb); }\
} while(false)


#define __MPROC(NAME) do {\
__FILL(); __MPV1(NAME);   \
} while(false)

//__FILL(); __MPV2(NAME);

    template <typename T,
    typename U,
    typename V>
    void doOPS(concurrent::for_each *loop)
    {
        std::cerr << "<OPS " << type_name_of<T>() << "," << type_name_of<U>()<< "," << type_name_of<V>() << ">" << std::endl;
        const T zt = 0;
        const U zu = 0;
        const V zv = 0;
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t n = 1000 + alea.leq(1000);
            //const size_t n = 100 + alea.leq(100);

            vector<T>    t(n,zt);
            vector<T>    tb(n,zt);
            vector<U>    u(n,zu);
            vector<V>    v(n,zv);

            __PROC(add);
            __MPROC(muladd);
            
            //__PROC(sub); __MPROC(mulsub);
            //__PROC(subp);

        }
        std::cerr << "<OPS/>" << std::endl;

    }

}

Y_UTEST(quark1_add)
{
    concurrent::simd loop;
    std::cerr << "With #Threads=" << loop.number() << std::endl;
    doOPS<float,float,float>( &loop );
    doOPS<float,int,float>( &loop );
    //doOPS<mpz,int,mpz>(NULL);

}
Y_UTEST_DONE()

