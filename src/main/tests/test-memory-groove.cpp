#include "y/memory/groove.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void doTest( memory::groove &g, const size_t n )
    {
        std::cerr << "\tmake<" << type_name_of<T>() << ">(" << n << "):";
        g.make<T>(n,memory::storage::shared); std::cerr << ' ' << g;
        g.make<T>(n,memory::storage::pooled); std::cerr << ' ' << g;
        g.make<T>(n,memory::storage::global); std::cerr << ' ' << g;


        std::cerr << std::endl;
    }

    static inline void doTests( memory::groove &g, const size_t n )
    {
        std::cerr << "test with #" << n << std::endl;
        doTest<double>(g,n);
        doTest<int32_t>(g,n);
        doTest<string>(g,n);
        doTest<mpn>(g,n);
    }
}

Y_UTEST(groove)
{
    std::cerr << "storage:" << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::unused) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::shared) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::pooled) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::global) << std::endl;
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(memory::groove);

    memory::groove g; std::cerr << g << std::endl;

    for(size_t n=1;n<=32;++n)
    {
        std::cerr << '\t';
        g.acquire(n,memory::storage::shared); std::cerr << g << ' ';
        g.acquire(n,memory::storage::pooled); std::cerr << g << ' ';
        g.acquire(n,memory::storage::global); std::cerr << g << ' ';
        std::cerr << std::endl;
    }

    g.release(); std::cerr << g << std::endl;
    std::cerr << std::endl;
    
    for(size_t n=0;n<=10;++n)
    {
        doTests(g,n);
    }



}
Y_UTEST_DONE()

