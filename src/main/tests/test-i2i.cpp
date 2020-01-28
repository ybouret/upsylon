#include "y/type/int2int.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace  {

    template <typename TARGET,typename SOURCE>
    void doTest(const SOURCE value)
    {
        std::cerr << "target=" << type_name_of<TARGET>() << std::endl;
        std::cerr << "     |_" << type_name_of< typename integral_of<TARGET>::type >() << std::endl;
        std::cerr << "source=" << type_name_of<SOURCE>() << std::endl;
        std::cerr << "     |_" << type_name_of< typename integral_of<SOURCE>::type >() << std::endl;
        
        const TARGET tgt = i2i<TARGET,SOURCE>(value);
        std::cerr << "i2i(" << value << ")=" << tgt << std::endl;
    }

}

Y_UTEST(i2i)
{
    doTest<unsigned char, unsigned char>( 'a' );
    doTest<unsigned char, unsigned short>( 'b' );
    doTest<unsigned char, uint32_t>( 'c' );
    doTest<unsigned char, uint64_t>( 'd' );

    doTest<unsigned long long, uint8_t>( 100 );
    doTest<uint64_t, uint16_t>( 10000 );
    doTest<uint64_t, uint32_t>( 10000 );
    doTest<uint64_t, uint64_t>( 10000 );


}
Y_UTEST_DONE()


