#include "y/os/uuid.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(uuid)
{
    std::cerr << "uuid::BITS  = " << uuid::BITS  << std::endl;
    std::cerr << "uuid::BYTES = " << uuid::BYTES << std::endl;

    for(size_t i=0;i<10;++i)
    {
        uuid ID;
        std::cerr << ID << std::endl;
    }
}
Y_UTEST_DONE()


