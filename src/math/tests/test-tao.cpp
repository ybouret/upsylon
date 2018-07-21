#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "support.hpp"
#include <typeinfo>
using namespace upsylon;
using namespace math;

namespace
{
    static const size_t nmin = 1024;
    static const size_t nmax = 32768;



    template <typename T>
    double do_test_ld(const size_t n, concurrent::for_each *loop)
    {
        static const size_t ITER_MAX = (sizeof(T)>16) ? 8 : 64;
        // ld
        rt_clock clk;
        std::cerr << "-- ld<" << typeid(T).name() << ">" << "," << n << ", " << (loop? "PAR" : "SEQ") << ": ";

        uint64_t  tmx=0;
        vector<T> a(n);
        for(size_t iter=0;iter<ITER_MAX;++iter)
        {
            const T   v = support::get<T>();

            {
                const uint64_t mark = rt_clock::ticks();
                tao::ld(a,v,loop);
                tmx += rt_clock::ticks()-mark;
            }
        }
        std::cerr <<  clk(tmx) << std::endl;
        return clk(tmx);
    }

    template <typename T,typename U,void (*OP)(array<T>&,const array<U>&,concurrent::for_each*)>
    double do_test_(const size_t n,concurrent::for_each *loop)
    {
        static const size_t ITER_MAX = (sizeof(T)>16) ? 8 : 64;

        rt_clock clk;

        std::cerr << "-- OP<" << typeid(T).name() << ">" << "," << n << ", " << (loop? "PAR" : "SEQ") << ": ";
        vector<T> a(n);
        vector<U> b(n);
        for(size_t i=n;i>0;--i) b[i] = support::get<U>();
        uint64_t tmx = 0;
        for(size_t iter=0;iter<ITER_MAX;++iter)
        {
            const uint64_t mark = rt_clock::ticks();
            OP(a,b,loop);
            tmx += rt_clock::ticks()-mark;
        }
        std::cerr <<  clk(tmx) << std::endl;
        return clk(tmx);
    }

    template <typename T,typename U,void (*OP)(array<T>&, typename type_traits<T>::parameter_type, const array<U>&,concurrent::for_each*)>
    double do_test2_(const size_t n,concurrent::for_each *loop)
    {
        static const size_t ITER_MAX = (sizeof(T)>16) ? 8 : 64;

        rt_clock clk;

        std::cerr << "-- OP<" << typeid(T).name() << ">" << "," << n << ", " << (loop? "PAR" : "SEQ") << ": ";
        vector<T> a(n);
        vector<U> b(n);
        for(size_t i=n;i>0;--i) b[i] = support::get<U>();
        uint64_t tmx = 0;
        for(size_t iter=0;iter<ITER_MAX;++iter)
        {
            const T x = support::get<T>();
            const uint64_t mark = rt_clock::ticks();
            OP(a,x,b,loop);
            tmx += rt_clock::ticks()-mark;
        }
        std::cerr <<  clk(tmx) << std::endl;
        return clk(tmx);
    }

    template <typename T,typename U,typename V> static inline
    double do_test_dot(const size_t n, concurrent::for_each *loop)
    {
        rt_clock clk;
        std::cerr << "-- dot<" << typeid(T).name() << ">" << "," << n << ", " << (loop? "PAR" : "SEQ") << ": ";

        vector<U> a(n);
        vector<V> b(n);
        static const size_t ITER_MAX = (sizeof(T)>16) ? 8 : 64;

        uint64_t tmx = 0;
        for(size_t iter=0;iter<ITER_MAX;++iter)
        {
            for(size_t i=n;i>0;--i)
            {
                a[i] = support::get<U>();
                b[i] = support::get<V>();
            }
            const uint64_t mark = rt_clock::ticks();
            (void) tao::dot<T,U,V>(a,b,loop);
            tmx += rt_clock::ticks()-mark;
        }
        std::cerr <<  clk(tmx) << std::endl;

        return clk(tmx);
    }


    template <typename T,typename U,typename V> static inline
    void do_test_dot_eq(const size_t n, concurrent::for_each &loop)
    {
        vector<U> a(n);
        vector<V> b(n);


        for(size_t i=n;i>0;--i)
        {
            a[i] = support::get<U>();
            b[i] = support::get<V>();
        }

        const T ans_seq = tao::_dot<T,U,V>(a,b);
        const T ans_par = tao::_dot<T,U,V>(a,b,loop);
        //std::cerr << "-- dot<" << typeid(T).name() << ">: ";
        Y_ASSERT(ans_seq==ans_par);
    }

}


#define _PAR(CODE) do { for(size_t n=nmin;n<=nmax;n*=2) { const double tseq = CODE(n,NULL); const double tpar = CODE(n,&loop); std::cerr << "\t\tSpeedUp=" << tseq/tpar << std::endl; } } while(false)
#define _SEQ(CODE) do { for(size_t n=nmin;n<=nmax;n*=2) { (void)CODE(n,NULL); } } while(false)
Y_UTEST(tao)
{
    concurrent::simd loop;

    if(true)
    {
        _PAR(do_test_ld<float>);
        _PAR(do_test_ld<double>);
        _SEQ(do_test_ld<mpn>);

        _PAR((do_test_<float,float,tao::set>));
        _PAR((do_test_<double,float,tao::set>));
        _PAR((do_test_<double,double,tao::set>));
        _SEQ((do_test_<mpz,int,tao::set>));

        _PAR((do_test_<float,float,tao::add>));
        _PAR((do_test_<double,float,tao::add>));
        _PAR((do_test_<double,double,tao::add>));
        _SEQ((do_test_<mpn,uint16_t,tao::add>));

        _PAR((do_test_<float,float,tao::sub>));
        _PAR((do_test_<double,float,tao::sub>));
        _PAR((do_test_<double,double,tao::sub>));
        _SEQ((do_test_<mpz,int16_t,tao::sub>));

        _PAR((do_test2_<float,float,tao::muladd>));
        _PAR((do_test2_<complex<double>,float,tao::muladd>));
        _SEQ((do_test2_<mpn,uint32_t,tao::muladd>));

        _PAR((do_test2_<float,float,tao::mulsub>));
        _PAR((do_test2_<complex<double>,float,tao::mulsub>));
        _SEQ((do_test2_<mpz,int32_t,tao::mulsub>));

        _PAR((do_test2_<float,float,tao::mulset>));
        _PAR((do_test2_<complex<double>,float,tao::mulset>));
        _SEQ((do_test2_<mpz,int32_t,tao::mulset>));

        _PAR((do_test_dot<double,double,float>));
    }

    std::cerr << "-- Testing Dot Equality" << std::endl;
    for(size_t n=0;n<4000;++n)
    {
        do_test_dot_eq<uint64_t,uint16_t,uint16_t>(n,loop);
    }
}
Y_UTEST_DONE()

