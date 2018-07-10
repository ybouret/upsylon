#include "y/code/xbitrev.hpp"
#include "y/utest/run.hpp"
#include "y/memory/dyadic.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        typedef complex<T> cplx;
        for(size_t n=1;n<=4096;n*=2)
        {
            vector<cplx,memory::dyadic> v(n);
            xbitrev::run_safe( &v[1].re-1,v.size());
        }
    }

    static inline void generate(const size_t size)
    {

        {
            std::cerr << "size=" << size << std::endl;
            vector<size_t> idx(size,as_capacity);
            vector<size_t> jdx(size,as_capacity);

            const size_t n = size << 1;
            size_t       j = 1;
            for( size_t i=1; i<n; i+=2)
            {
                if(j>i)
                {
                    //core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
                    //std::cerr << "(" << i << "," << j << ")" << std::endl;
                    idx.push_back(i);
                    jdx.push_back(j);
                }
                size_t m = size;
                while( (m>=2) && (j>m) )
                {
                    j -=  m;
                    m >>= 1;
                }
                j += m;
            }
            const size_t nx = idx.size();
            assert(jdx.size()==idx.size());
            std::cerr << "\tnx=" << nx << std::endl;
        }
    }
}

Y_UTEST(xbitrev)
{
    do_test<float>();
    do_test<double>();

    generate(0);
    for(size_t size=1;size<=4096;size*=2)
    {
        generate(size);
    }

}
Y_UTEST_DONE()

