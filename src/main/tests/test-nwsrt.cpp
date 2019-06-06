//#define Y_NWSRT_VERBOSE 1
#include "y/sort/nwsrt.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include <typeinfo>

using namespace upsylon;

#if 1
namespace
{
    template <typename T>
    static inline void do_test()
    {
        const char *id = typeid(T).name();

        vector<T>      v( nwsrt::max_size, as_capacity);
        vector<size_t> w( nwsrt::max_size, as_capacity);

        for(size_t n=0;n<=nwsrt::max_size;++n)
        {
            std::cerr << "n=" << n << ", <" << id << ">" << std::endl;
            for(size_t iter=0;iter<16;++iter)
            {
                v.free();
                w.free();
                for(size_t j=0;j<n;++j)
                {
                    const T x = support::get<T>();
                    v.push_back(x);
                    w.push_back(0);
                }
                
                for(size_t jter=0;jter<16;++jter)
                {
                    alea.shuffle(*v,n);
                    if(n<=3) std::cerr << "\tv0=" << v << std::endl;
                    nwsrt::on(*v,n);
                    if(n<=3) std::cerr << "\tv1=" << v << std::endl;

                    for(size_t j=1;j<n;++j)
                    {
                        Y_ASSERT(v[j]<=v[j+1]);
                    }

                    if(false)
                    {
                        alea.shuffle(*v,n);
                        for(size_t j=1;j<=n;++j) w[j] = j;
                        if(jter<=0&&iter<=0)
                        {
                            std::cerr << "v_raw=" << v << std::endl;
                            std::cerr << "w_raw=" << w << std::endl;
                        }
                        nwsrt::on(*v,*w,n);
                        if(jter<=0&&iter<=0)
                        {
                            std::cerr << "v_srt=" << v << std::endl;
                            std::cerr << "w_srt=" << w << std::endl;
                        }
                    }
                }
            }
        }
        
        
    }
}
#endif

Y_UTEST(nwsrt)
{
#if 1
    do_test<int32_t>();
    do_test<double>();
    do_test<string>();
#endif
}
Y_UTEST_DONE()

