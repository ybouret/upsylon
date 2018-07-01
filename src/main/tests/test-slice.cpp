#include "y/memory/slice.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(slice)
{
    std::cerr << "sizeof(slice::block)=" << sizeof(memory::slice::block) << std::endl;

    char buffer[200];
    memory::slice s(buffer,sizeof(buffer));
    s.display();

    size_t n = 1;
    s.acquire(n);
    s.display();
    
}
Y_UTEST_DONE()
