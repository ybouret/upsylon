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
        static const size_t ITER_MAX = (sizeof(T)>16) ? 16 : 128;
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
        static const size_t ITER_MAX = (sizeof(T)>16) ? 16 : 128;

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


}


#define _PAR(CODE) do { for(size_t n=nmin;n<=nmax;n*=2) { const double tseq = CODE(n,NULL); const double tpar = CODE(n,&loop); std::cerr << "\t\tSpeedUp=" << tseq/tpar << std::endl; } } while(false)
#define _SEQ(CODE) do { for(size_t n=nmin;n<=nmax;n*=2) { (void)CODE(n,NULL); } } while(false)
Y_UTEST(tao)
{
    concurrent::simd loop;

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

}
Y_UTEST_DONE()

