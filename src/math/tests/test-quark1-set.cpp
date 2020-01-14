
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
    void load( ARRAY &arr  )
    {
        for(size_t i=1;i<=arr.size();++i)
        {
            typename ARRAY::const_type tmp = support::get< typename ARRAY::type >();
            arr[i] = tmp;
        }
    }

    template <typename LHS, typename RHS>
    void check1D( const LHS &lhs, const RHS &rhs )
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            Y_ASSERT( __mod2(lhs[i]-rhs[i]) <= 0 );
        }
    }

    template <typename T>
    void doSET(concurrent::simd *loop)
    {
        std::cerr << "<SET " << typeid(T).name() << ">" << std::endl;

        concurrent::sequential_for seq;

        point2d<T> p2;
        point3d<T> p3;

        const T z(0);
        for(size_t n=1;n<=32;++n)
        {
            vector<T>                vg(n,z);
            vector<T,memory::pooled> vp(n,z);
            list<T>                  l(n,z);



            vector<T> tmp(n,z);

#define ___QUARK_SET_CORE(LHS,RHS) do { load(RHS); quark::set(LHS,RHS); check1D(LHS,RHS);       } while(false)
#define  __QUARK_SET_CORE(LHS,RHS) do { ___QUARK_SET_CORE(LHS,RHS); ___QUARK_SET_CORE(RHS,LHS); } while(false)

#define ___QUARK_SET_LOOP(LHS,RHS,LOOP) do { load(RHS); quark::set(LHS,RHS,LOOP); check1D(LHS,RHS); } while(false)
#define  __QUARK_SET_LOOP(LHS,RHS,LOOP) do { ___QUARK_SET_LOOP(LHS,RHS,LOOP); ___QUARK_SET_LOOP(RHS,LHS,LOOP); } while(false)

#define _QUARK_SET(ARR) do {\
__QUARK_SET_CORE(tmp,ARR);\
__QUARK_SET_LOOP(tmp,ARR,seq);\
if(loop) { __QUARK_SET_LOOP(tmp,ARR,*loop); }\
} while(false)

            _QUARK_SET(vg);
            _QUARK_SET(vp);
            _QUARK_SET(l);
            if(n==2) _QUARK_SET(p2);
            if(n==3) _QUARK_SET(p3);
            

        }
        std::cerr << "<SET/>" << std::endl;

    }

}

Y_UTEST(quark1_set)
{
    concurrent::simd           loop;

    doSET<float>(  &loop );
    doSET<double>( &loop );
    doSET< complex<float>  >(  &loop );
    doSET< complex<double> >(  &loop );

    doSET< mpn >(NULL);
    doSET< mpz >(NULL);
    doSET< mpq >(NULL);

}
Y_UTEST_DONE()

