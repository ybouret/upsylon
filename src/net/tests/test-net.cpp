#include "y/net/net.hpp"
#include "y/net/types.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;

template <typename T>
static inline void test_nbo( const T x )
{
    const T y = net::swap_nbo_as(x);
    const T z = net::swap_nbo_as(y);
    std::cerr << "[" << typeid(T).name() << "] : " << x << " -> " << y << " -> " << z << std::endl;
    Y_ASSERT(z==x);

}

Y_UTEST(network)
{
    network::verbose  = true;
    network      & nw = network::instance();


    net128_t x;
    std::cerr << "x=" << x << std::endl;

    std::cerr << std::hex;
    for(size_t i=0;i<8;++i)
    {
        test_nbo( alea.full<int>()   );
        test_nbo( alea.full<short>() );
        test_nbo( alea.full<uint64_t>() );
        test_nbo( alea.full<int32_t>() );
    }

    const string   hn = nw.get_host_name();
    std::cerr << "host_name=" << hn << std::endl;

    const net::exception excp(0,"testing exception");
    std::cerr << "excp.code=" << excp.code() << " [" << excp.what() << "] while " << excp.when() << std::endl;
}
Y_UTEST_DONE()

