
#include "y/math/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename ARRAY>
    void load( ARRAY &arr  )
    {
        support::fill1D(arr);
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


    template <typename T, typename U> static inline
    void checkCast( const accessible<T> &t, const accessible<U> &u )
    {
        assert(t.size()==u.size());
        for(size_t i=t.size();i>0;--i)
        {
            const T tt = t[i];
            const T uu = auto_cast<T,U>::_(u[i]);
            if( __mod2(tt-uu) > 0 )
            {
                std::cerr << type_name_of<T>() << " : " << tt << "!=" << uu << " from " << type_name_of<U>() << " = " << u[i] << std::endl;
            }
            Y_ASSERT( __mod2(tt-uu) <= 0 || die("set failure") );
        }
    }

    template <typename T, typename U> static inline
    void checkCast( const accessible<T> &t, const T &x, const accessible<U> &u )
    {
        assert(t.size()==u.size());
        for(size_t i=t.size();i>0;--i)
        {
            const T tt = t[i];
            const T uu =  x * auto_cast<T,U>::_(u[i]);
            Y_ASSERT( __mod2(tt-uu) <= 0  || die("mulset failure") );
        }
    }


    template <typename T, typename U>
    static inline void doSET2(concurrent::simd *loop)
    {
        std::cerr << "<SET/MULSET " << typeid(T).name() << " <-- " << typeid(U).name() << ">" << std::endl;
        const U zu = 0;
        const T zt = 0;
        const size_t n = 10 + alea.leq(1000);
        vector<T> t(n,zt);
        vector<U> u(n,zu);
        std::cerr << "\t[SEQUENTIAL]" << std::endl;
        load(u); quark::set(t,u); checkCast(t,u);
        const T x = support::get<T>();
        load(u); quark::mulset(t,x,u); checkCast(t,x,u);
        if(loop)
        {
            std::cerr << "\t[PARALLEL]" << std::endl;
            load(u); quark::set(t,u,*loop);       checkCast(t,u);
            load(u); quark::mulset(t,x,u,*loop);  checkCast(t,x,u);
        }
        std::cerr << "<SET/MULSET/>" << std::endl;
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

    doSET2<float,int>( &loop );
    doSET2<double,short>( &loop );
    doSET2<mpn,unsigned>( NULL );
    doSET2<mpz,int>( NULL );
    doSET2<mpq,int>( NULL );


}
Y_UTEST_DONE()

