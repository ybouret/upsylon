#include "y/memory/groove.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

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



}
Y_UTEST_DONE()

