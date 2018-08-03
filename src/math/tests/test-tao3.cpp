
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
    template <typename T,typename U,typename V> static inline
    double do_mmul(const size_t n, const size_t m, concurrent::for_each *loop)
    {
        std::cerr << "mmul " << n  << "x" << m << " : ";
        rt_clock  clk;
        uint64_t  tmx=0;
        matrix<T> M(n,m);
        for(size_t p=1;p<=32;++p)
        {
            matrix<U> A(n,p);
            matrix<V> B(p,m);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=p;++j)
                {
                    A[i][j] = support::get<U>();
                }
            }
            for(size_t i=1;i<=p;++i)
            {
                for(size_t j=1;j<=m;++j)
                {
                    B[i][j] = support::get<V>();
                }
            }
            const uint64_t mark = rt_clock::ticks();
            tao::mmul(M,A,B,loop);
            tmx += rt_clock::ticks() - mark;
        }
        std::cerr << clk(tmx) << std::endl;
        return clk(tmx);
    }
    
    
    template <typename T,typename U,typename V> static inline
    double do_mmul_rtrn(const size_t n,
                        const size_t m,
                        concurrent::for_each *loop)
    {
        std::cerr << "mmul " << n  << "x" << m << " : ";
        rt_clock  clk;
        uint64_t  tmx=0;
        matrix<T> M(n,m);
        for(size_t p=1;p<=32;++p)
        {
            matrix<U> A(n,p);
            matrix<V> B(m,p);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=p;++j)
                {
                    A[i][j] = support::get<U>();
                }
            }
            for(size_t i=1;i<=m;++i)
            {
                for(size_t j=1;j<=p;++j)
                {
                    B[i][j] = support::get<V>();
                }
            }
            
            const uint64_t mark = rt_clock::ticks();
            tao::mmul_rtrn(M,A,B,loop);
            tmx += rt_clock::ticks() - mark;
        }
        std::cerr << clk(tmx) << std::endl;
        return clk(tmx);
    }
    
    
}

#define LOOP(CODE) do { \
const double tseq = CODE(r,c,NULL);  \
const double tpar = CODE(r,c,&loop); \
std::cerr << "\t" << #CODE << " speedup: " << tseq/tpar << std::endl;\
} while(false)

Y_UTEST(tao3)
{
    concurrent::simd loop;
    
    for(size_t r=1;r<=100;r+=1+alea.leq(20))
    {
        for(size_t c=1;c<=100;c+=1+alea.leq(20))
        {
            LOOP( (do_mmul<float,float,float>) );
            LOOP( (do_mmul_rtrn<double,double,double>) );
            matrix<unit_t> Mseq(r,c);
            matrix<unit_t> Mpar(r,c);
            for(size_t p=1;p<=32;++p)
            {
                {
                    matrix<int> A(r,p);
                    matrix<int> B(c,p);
                    for(size_t i=1;i<=r;++i)
                    {
                        for(size_t j=1;j<=p;++j)
                        {
                            A[i][j] = support::get<int>();
                        }
                    }
                    for(size_t i=1;i<=c;++i)
                    {
                        for(size_t j=1;j<=p;++j)
                        {
                            B[i][j] = support::get<int>();
                        }
                    }
                    tao::mmul_rtrn(Mseq,A,B,0);
                    tao::mmul_rtrn(Mpar,A,B,&loop);
                    const lightweight_array<unit_t> arr_seq = Mseq.as_array();
                    const lightweight_array<unit_t> arr_par = Mpar.as_array();
                    const unit_t diff = tao::mod2(arr_seq,arr_par);
                    Y_ASSERT(0==diff);
                }
            }
        }
        
    }
    

}
Y_UTEST_DONE()
