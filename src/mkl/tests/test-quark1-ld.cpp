
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename ARRAY>
    static inline void reset1D( ARRAY &arr )
    {
        typename ARRAY::const_type z(0);
        for(size_t i=arr.size();i>0;--i) arr[i] = z;
    }

    template <typename ARRAY>
    static inline void check1D( const ARRAY &arr, typename ARRAY::const_type &value )
    {
        std::cerr << "\tCheck for " << type_name_of<ARRAY>() << std::endl;
        for(size_t i=arr.size();i>0;--i)
        {
            Y_ASSERT( __mod2( arr[i] - value ) <= 0 );
        }
    }

    template <typename T>
    void doLD( concurrent::simd *par )
    {
        concurrent::sequential_for seq;
        std::cerr << "<LD " << type_name_of<T>() << ">" << std::endl;
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
#define _QUARK_LD(S)                         do { \
reset1D(S); quark::ld(S,tmp);     check1D(S,tmp); \
reset1D(S); quark::ld(S,tmp,seq); check1D(S,tmp); \
if(par) { reset1D(S); quark::ld(S,tmp,*par); check1D(S,tmp); } \
} while(false)

        _QUARK_LD(vg);
        _QUARK_LD(vp);
        _QUARK_LD(l);
        _QUARK_LD(p2);
        _QUARK_LD(p3);


        std::cerr << "<LD/>" << std::endl << std::endl;

    }

}


Y_UTEST(quark1_ld)
{
    concurrent::simd           loop;

    doLD< float >(&loop);
    doLD< double >(&loop);
    doLD< complex<float> >(&loop);
    doLD< complex<double> >(&loop);

    doLD<mpn>(NULL);
    doLD<mpz>(NULL);
    doLD<mpq>(NULL);

}
Y_UTEST_DONE()


