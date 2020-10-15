#include "y/mkl/tao.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    template <typename T> static inline
    void fill( sequence<T> &seq, size_t n)
    {
        seq.free();
        while(n-- > 0)
        {
            const T tmp = support::get<T>();
            seq.push_back(tmp);
        }
    }

    template <typename LHS> static inline
    bool checkValue(const LHS &lhs, typename LHS::param_type value)
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            if( fabs_of(lhs[i]-value) > 0 )
            {
                std::cerr << "Bad Check Value for <" << type_name_of<typename LHS::mutable_type>() << ">" << std::endl;
                return false;
            }
        }
        return true;
    }


    //==========================================================================
    //
    // LD
    //
    //==========================================================================
    template <typename LHS> static inline
    void test_ld__(LHS &lhs)
    {
        typedef typename LHS::mutable_type type;
        {
            const type value = support::get<type>();
            tao::ld(lhs,value);
            Y_ASSERT( checkValue(lhs,value) );
        }
        {
            const type value = support::get<type>();
            tao::ld(lhs,value,alea.leq(lhs.size()));
        }
    }

    template <typename T> static inline
    void test_ld_()
    {
        std::cerr << "ld<" << type_name_of<T>() << ">" << std::endl;
        {
            const T x = support::get<T>();
            const T y = support::get<T>();
            const T z = support::get<T>();
            {
                point2d<T> p(x,y);
                test_ld__(p);
            }

            {
                point3d<T> p(x,y,z);
                test_ld__(p);
            }
        }

        for(size_t n=1;n<=128;n <<= 1)
        {
            {
                vector<T> vg(n,as_capacity);
                fill(vg,n);
                test_ld__(vg);
            }

            {
                list<T>   lg(n,as_capacity);
                fill(lg,n);
                test_ld__(lg);
            }
        }
    }

    static inline
    void test_ld()
    {
        test_ld_<ptrdiff_t>();
        test_ld_<float>();
        test_ld_<double>();
        test_ld_<apn>();
        test_ld_<apz>();
        test_ld_<apq>();
    }

    //==========================================================================
    //
    // SET
    //
    //==========================================================================

    template <typename LHS, typename RHS> static inline
    void test_set__( LHS &lhs, RHS &rhs )
    {
        tao::set(lhs,rhs);
        tao::upload(rhs,lhs);
    }

    template <typename T, typename U> static inline
    void test_set_()
    {
        std::cerr << "set<" << type_name_of<T>() << "," << type_name_of<U>() << ">" << std::endl;

        {
            const T xt = support::get<T>();
            const T yt = support::get<T>();
            const T zt = support::get<T>();

            const U xu = support::get<U>();
            const U yu = support::get<U>();
            const U zu = support::get<U>();

            point2d<T> p2t(xt,yt);
            point3d<T> q3t(xt,yt,zt);
            test_set__(p2t,q3t);


            point2d<U> p2u(xu,yu);
            point3d<U> q3u(xu,yu,zu);
            test_set__(p2u,q3u);

            test_set__(p2t,q3u);
        }

        for(size_t n=1;n<=128;n <<= 1)
        {
            vector<T> v(n,as_capacity);
            list<U>   l;
            fill(v,n);
            fill(l,n+alea.leq(16));
            test_set__(v,l);

        }

    }

    static inline
    void test_set()
    {
        test_set_<ptrdiff_t,ptrdiff_t>();
        test_set_<double,ptrdiff_t>();
        test_set_<apn,apn>();
    }

}

Y_UTEST(tao)
{
    test_ld();   std::cerr << std::endl;
    test_set();  std::cerr << std::endl;
}
Y_UTEST_DONE()

