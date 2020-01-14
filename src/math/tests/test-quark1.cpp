
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
    static inline void reset1D( ARRAY &arr )
    {
        for(size_t i=arr.size();i>0;--i) arr[i] = 0;
    }

    template <typename ARRAY>
    static inline void check1D( const ARRAY &arr, typename ARRAY::const_type &value )
    {
        std::cerr << "\tCheck " << typeid(ARRAY).name() << std::endl;
        for(size_t i=arr.size();i>0;--i)
        {
            Y_ASSERT(0==memcmp(&arr[i], &value, sizeof( typename ARRAY::const_type  ) ));
        }
    }

    template <typename T>
    void doLD( concurrent::simd &par )
    {
        concurrent::sequential_for seq;
        std::cerr << "<LD " << typeid(T).name() << ">" << std::endl;
        vector<T>  vg;
        vector<T>  vp;
        list<T>    l;
        point2d<T> p2;
        point3d<T> p3;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            vg.push_back( support::get<T>() );
        }

        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            vp.push_back( support::get<T>() );
        }

        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            l.push_back( support::get<T>() );
        }

        const T tmp = support::get<T>();
#define _QUARK_LD(S)                              \
reset1D(S); quark::ld(S,tmp);     check1D(S,tmp); \
reset1D(S); quark::ld(S,tmp,seq); check1D(S,tmp); \
reset1D(S); quark::ld(S,tmp,par); check1D(S,tmp);

        _QUARK_LD(vg);
        _QUARK_LD(vp);
        _QUARK_LD(l);
        _QUARK_LD(p2);
        _QUARK_LD(p3);


        std::cerr << "<LD/>" << std::endl << std::endl;

    }

}


Y_UTEST(quark1)
{
    concurrent::simd           loop;

    doLD< float >(loop);
    doLD< double >(loop);
    doLD< complex<float> >(loop);
    doLD< complex<double> >(loop);

}
Y_UTEST_DONE()


