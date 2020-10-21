#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/tao.hpp"
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
                return false;
            }
        }
        return true;
    }


    template <typename LHS, typename RHS> static inline
    bool check1D(const LHS &lhs, const RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        for(size_t i=lhs.size();i>0;--i)
        {
            if( fabs_of(lhs[i]-Y_TAO_CAST(LHS,RHS,rhs[i])) > 0 )
            {
                std::cerr << lhs[i] << " / " << rhs[i] << std::endl;
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
    void test_ld__(LHS &lhs, const bool check)
    {
        typedef typename LHS::mutable_type type;
        {
            const type value = support::get<type>();
            tao::ld(lhs,value);
            if(check) Y_ASSERT( checkValue(lhs,value) );
        }
        {
            const type value = support::get<type>();
            tao::ld(lhs,value,alea.leq(lhs.size()));
        }
    }

    template <typename T> static inline
    void test_ld_(const bool check)
    {
        std::cerr << "ld<" << type_name_of<T>() << ">" << std::endl;
        {
            const T x = support::get<T>();
            const T y = support::get<T>();
            const T z = support::get<T>();
            {
                point2d<T> p(x,y);
                test_ld__(p,check);
            }

            {
                point3d<T> p(x,y,z);
                test_ld__(p,check);
            }
        }

        for(size_t n=1;n<=128;n <<= 1)
        {
            {
                vector<T> vg(n,as_capacity);
                fill(vg,n);
                test_ld__(vg,check);
            }

            {
                list<T>   lg(n,as_capacity);
                fill(lg,n);
                test_ld__(lg,check);
            }
        }
    }

    static inline
    void test_ld()
    {
        test_ld_<ptrdiff_t>(true);
        test_ld_<float>(false);
        test_ld_<double>(false);
        test_ld_<apn>(true);
        test_ld_<apz>(true);
        test_ld_<apq>(true);
    }

    //==========================================================================
    //
    // SET
    //
    //==========================================================================

    template <typename LHS, typename RHS> static inline
    void test_set__( LHS &lhs, RHS &rhs )
    {
        tao::set(lhs,rhs);    Y_ASSERT(check1D(lhs,rhs));
        support::fill1D(rhs);
        tao::upload(rhs,lhs); Y_ASSERT(check1D(lhs,rhs));
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


    //==========================================================================
    //
    // ADD/SUB
    //
    //==========================================================================
    template <typename T, typename U, typename W> static inline
    void test_add_()
    {
        std::cerr << "addops<" << type_name_of<T>() << "," << type_name_of<U>() << "," << type_name_of<W>() <<">" << std::endl;

        for(size_t n=1;n<=128;n <<= 1)
        {
            vector<T> a;
            vector<T> s;
            list<U>   l;
            vector<W> r;
            fill(a,n);
            fill(s,n);
            fill(l,n);
            fill(r,n);
            tao::add(a,l,r);
            tao::sub(s,l,r);
            tao::sub(a,r);
            tao::add(s,r);
            Y_ASSERT(check1D(a,s));
            tao::sub(a,l,r);
            tao::sub(s,r,l);
            tao::neg(s);
            Y_ASSERT(check1D(a,s));
            tao::set(a,l);
            tao::subp(a,r);
            tao::sub(s,r,l);
            Y_ASSERT(check1D(a,s));
        }
    }

    static inline
    void test_add()
    {
        test_add_<apz,apz,apz>();
        test_add_<apq,apq,apq>();
    }

    //==========================================================================
    //
    // MUL/DIV
    //
    //==========================================================================
    template <typename T> static inline
    void test_mul_(const bool check)
    {
        std::cerr << "mulops<" << type_name_of<T>() << ">" << std::endl;
        for(size_t n=1;n<=128;n <<= 1)
        {
            vector<T> p;
            list<T>   l;
            vector<T> r;
            vector<T> q;
            fill(p,n);
            fill(l,n);
            fill(r,n);

            {
                const T value = support::get<T>();
                tao::mulset(p, value, l);
                tao::set(r,l);
                tao::mulset(r,value);
                if(check)
                {
                    Y_ASSERT(check1D(p,r));
                }
            }

            fill(p,n);
            fill(l,n);
            fill(r,n);
            fill(q,n);

            {
                const T value = support::get<T>();
                tao::muladd(p, l, value, r);
                tao::set(q,l);
                tao::muladd(q,value,r);
                if(check)
                {
                    Y_ASSERT(check1D(p,q));
                }
            }
        }
    }

    static inline
    void test_mul()
    {
        test_mul_<float>(false);
        test_mul_<double>(false);
        test_mul_< complex<float>  >(false);
        test_mul_< complex<double> >(false);
        test_mul_<apq>(true);
    }


    //==========================================================================
    //
    // dot/mod2
    //
    //==========================================================================
    template <typename T> static inline
    void test_dot_(const bool check)
    {
        std::cerr << "dot<" << type_name_of<T>() << ">" << std::endl;
        vector<T> l(1024,as_capacity);
        list<T>   r(1024,as_capacity);
        for(size_t n=1;n<=64;n <<= 1)
        {
            fill(l,n);
            fill(r,n);
            const T lr = tao::dot<T>::of(l,r);
            const T rl = tao::dot<T>::of(r,l);
            if(check)
            {
                Y_ASSERT(lr==rl);
            }
        }
    }

    static inline
    void test_dot()
    {
        test_dot_<float>(false);
        test_dot_<double>(false);
        test_dot_< complex<float>  >(false);
        test_dot_< complex<double> >(false);
        test_dot_<apq>(true);
    }

    //==========================================================================
    //
    // mulv2
    //
    //==========================================================================

    template <typename T, typename U, typename W> static inline
    void test_mul_v2_()
    {
        std::cerr << "mulv2<" << type_name_of<T>() << "," << type_name_of<U>() << "," << type_name_of<W>() <<">" << std::endl;
        for(size_t r=1;r<=64;r <<= 1)
        {
            for(size_t c=1;c<=64;c <<= 1)
            {

                {
                    vector<T> vr;
                    vector<W> vc;
                    fill(vr,r);
                    fill(vc,c);
                    matrix<U> M(r,c);
                    support::fill2D(M);
                    tao::mul(vr,M,vc);
                }

                {
                    vector<T> vc;
                    vector<W> vr;
                    fill(vr,r);
                    fill(vc,c);
                    matrix<U> M(c,r);
                    support::fill2D(M);
                    tao::mul(vc,M,vr);
                }
            }
        }
    }

    static inline
    void test_mv2()
    {
        test_mul_v2_<double,int16_t,double>();
        test_mul_v2_<apz,int16_t,int32_t>();
    }


    template <typename T, typename U, typename W> static inline
    void test_mul_v3_()
    {
        std::cerr << "mulv3<" << type_name_of<T>() << "," << type_name_of<U>() << "," << type_name_of<W>() <<">" << std::endl;

        for(size_t r=1;r<=64;r <<= 1)
        {
            for(size_t c=1; c <= 64; c <<= 1)
            {
                matrix<T> M(r,c);
                for(size_t n=1; n <= 128; n <<= 1)
                {
                    matrix<U> A(r,n);
                    {
                        matrix<W> B(n,c);
                        tao::mmul(M,A,B);
                    }
                    {
                        matrix<W> B(c,n);
                        tao::mmul_rtrn(M,A,B);
                    }
                }
            }
        }
    }



    static inline
    void test_mv3()
    {
        test_mul_v3_<double,int16_t,float>();
        test_mul_v3_<apz,int16_t,int32_t>();
    }

    template <typename T, typename U>
    void test_gram_(const bool  check)
    {
        std::cerr << "gram<" << type_name_of<T>() << "," << type_name_of<U>() << ">" << std::endl;
        for(size_t r=1;r<=64;r <<= 1)
        {
            matrix<T> M(r,r);
            matrix<T> MM(r,r);
            for(size_t c=1;c<=128;c <<= 1)
            {
                matrix<U> A(r,c);
                support::fill2D(A);
                tao::gram(M,A);
                if(check)
                {
                    tao::mmul_rtrn(MM,A,A);
                    Y_ASSERT( tao::mmod2<T>::of(M,MM) <= 0);
                }
            }
        }
    }

    static inline
    void test_gram()
    {
        test_gram_<double,float>(false);
        test_gram_<apz,int16_t>(true);
    }

}

Y_UTEST(tao)
{
    test_ld();    std::cerr << std::endl;
    test_set();   std::cerr << std::endl;
    test_add();   std::cerr << std::endl;
    test_mul();   std::cerr << std::endl;
    test_dot();   std::cerr << std::endl;
    test_mv2();   std::cerr << std::endl;
    test_mv3();   std::cerr << std::endl;
    test_gram();  std::cerr << std::endl;

}
Y_UTEST_DONE()

