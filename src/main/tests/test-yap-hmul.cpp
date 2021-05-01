
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/type/complex.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/type/rtti.hpp"
#include "y/code/hr-ints.hpp"
#include <cstring>

using namespace upsylon;

namespace
{


#define Y_HMUL_MARK() static const size_t MARK = __LINE__  + 1
#define Y_HMUL_INDX   (__LINE__-MARK)
#define Y_HMUL_IMPL() L[Y_HMUL_INDX] *= R[Y_HMUL_INDX]

    template <typename T> static inline
    void h_safe(T *L, const T *R, const size_t N) throw()
    {
        for(size_t i=0;i<N;++i) L[i] *= R[i];
    }

#define Y_HMUL_INIT(N) case N: { Y_HMUL_MARK()
#define Y_MMUL_QUIT()  } return

    template <typename T>
    void h_vmul(T *L, const T *R, const size_t N) throw()
    {
        switch(N)
        {
                Y_HMUL_INIT(1);
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();

                Y_HMUL_INIT(2);
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();

                Y_HMUL_INIT(4);
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();

                Y_HMUL_INIT(8);
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();

                Y_HMUL_INIT(16);
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();

                Y_HMUL_INIT(32);
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_HMUL_IMPL();
                Y_MMUL_QUIT();


            default:
                for(size_t i=0;i<N;++i) L[i] *= R[i];
        }
    }

    template <typename T>
    static inline void do_test(const double D, const size_t pmax)
    {
        std::cerr << "h-mul with [" << rtti::name_of<T>() << "]" << std::endl;
        for(size_t p=0;p<=pmax;++p)
        {
            const size_t n = size_t(1) << p;
            vector<T>    lhs(n,0), rhs(n,0), seq(n,0);
            for(size_t i=n;i>0;--i)
            {
                lhs[i] = seq[i] = support::get<T>();
                rhs[i] = support::get<T>();
            }

            h_vmul(*lhs,*rhs,n);
            h_safe(*seq,*rhs,n);
            std::cerr << std::setw(8) << n << ' '; Y_CHECK( 0 == memcmp(*lhs,*seq,n*sizeof(T)));

            uint64_t        vmul = 0;
            uint64_t        safe = 0;
            real_time_clock clk;
            uint64_t        cycles = 0;
            do
            {
                ++cycles;
#if 0
                for(size_t i=n;i>0;--i)
                {
                    lhs[i] = seq[i] = support::get<T>();
                    rhs[i] = support::get<T>();
                }
#endif

                Y_RTC_ADD(vmul,  h_vmul(*lhs,*rhs,n) );
                Y_RTC_ADD(safe,  h_safe(*seq,*rhs,n) );


            } while( clk(vmul)<D && clk(safe)<D );
            cycles *= n;
            const int64_t vmul_speed = int64_t( floor( double(cycles)/clk(vmul) + 0.5) );
            const int64_t safe_speed = int64_t( floor( double(cycles)/clk(safe) + 0.5) );
            std::cerr << std::setw(8) << '-';
            std::cerr << " | vmul: " << human_readable(vmul_speed);
            std::cerr << " | safe: " << human_readable(safe_speed);
            std::cerr << " | rho : " << double(vmul_speed)/double(safe_speed);

            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }



}

#include "y/string/convert.hpp"
#include "y/concurrent/assign-main.hpp"

Y_UTEST(yap_hmul)
{
    (void) assign_main::by_pid(program);

    double D = 0.5;
    if(argc>1)
    {
        D = string_convert::to<double>(argv[1],"D");
    }
    size_t pmax=8;
    if(argc>2)
    {
        pmax = string_convert::to<size_t>(argv[2],"pmax");
    }
    //do_test<float>(D,pmax);
    do_test< complex<double> >(D,pmax);

}
Y_UTEST_DONE()
