#include "y/mkl/utils.hpp"
#include "y/mkl/show.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/mkl/gaussian.hpp"
#include "y/mkl/timings.hpp"

using namespace upsylon;
using namespace math;

namespace
{
#define ROUND(VAR) \
std::cerr << #VAR << "=" << VAR << std::endl; \
std::cerr << "\tlog_round_floor=" << timings::round_floor(VAR) << std::endl;\
std::cerr << "\tlog_round_ceil =" << timings::round_ceil(VAR) << std::endl

    template <typename T>
    void do_round()
    {
        std::cerr << std::endl << "-- Rounding" << std::endl;
        ROUND(numeric<T>::epsilon);
        std::cerr << std::endl << "-- Truncate" << std::endl;
        vector<T> v(1+alea.leq(100));
        for(size_t i=1;i<=v.size();++i)
        {
            v[i] = support::get<T>();
        }
        
        (void)__find<T>::truncate(v);
        matrix<T> A( 1+alea.leq(10), 1+alea.leq(10));
        for(size_t i=1;i<=A.rows;++i)
        {
            for(size_t j=1;j<=A.cols;++j)
            {
                A[i][j] = support::get<T>();
            }
        }
        __find<T>::truncate(A);
    }
}

Y_UTEST(types)
{

    do_round<float>();
    do_round<double>();

    vector<float> v( 1+alea.leq(10) );
    list<double>  l;
    vector< complex<float> > c;
    for(size_t i=1;i<=v.size();++i)
    {
        v[i] = support::get<float>()-0.5f;
        l.push_back(v[i]);
        c.push_back( support::get< complex<float> >() );
    }
    std::cerr << "v=" << v << std::endl;
    std::cerr << "l=" << l << std::endl;
    std::cerr << "c=" << c << std::endl;

    std::cerr << "max_of(v)    =" << __find<float>::max_of(v) << std::endl;
    std::cerr << "abs_max_of(v)=" << __find<float>::abs_max_of(v) << std::endl;
    std::cerr << "max_of(l)    =" << __find<double>::max_of(l) << std::endl;
    std::cerr << "abs_max_of(l)=" << __find<double>::abs_max_of(l) << std::endl;
    std::cerr << "abs_max_of(c)=" << __find<float>::abs_max_of(c) << std::endl;

    std::cerr << "Setting sizes" << std::endl;
    std::cerr << "v=" << v << std::endl;
    for(size_t i=0;i<=v.size();++i)
    {
        vector<float> V = v;
        __find<float>::set_image_size(V,i);
        std::cerr << "#img=" << i << " => " << V << std::endl;
    }
    
    for(size_t i=0;i<=v.size();++i)
    {
        vector<float> V = v;
        __find<float>::set_kernel_size(V,i);
        std::cerr << "#ker=" << i << " => " << V << std::endl;
    }
    
    math::show::info();

    std::cerr << timings::round_floor(0.12) << std::endl;
    std::cerr << timings::round_ceil(0.12)  << std::endl;

    float dt    = 0.11f;
    float save  = 0.3f;
    float t_run = 1.0f;
    std::cerr << "dt=" << dt << ", save=" << save << std::endl;
    const size_t every = timings::save_every(dt,save);
    std::cerr << "dt=" << dt << ", save=" << save << " every " << every << std::endl;
    const size_t iters = timings::iterations(t_run, dt, every);
    std::cerr << "Running #" << iters << " up to " << t_run << std::endl;
    
    
}
Y_UTEST_DONE()

