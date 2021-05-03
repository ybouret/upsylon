#include "y/associative/suffix/address.hpp"
#include "y/associative/suffix/xaddress.hpp"
#include "y/type/rtti.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    static inline void do_xaddr()
    {
        std::cerr << std::endl;
        std::cerr << "for <" << rtti::name_of<T>() << ">" << std::endl;

        std::cerr << "xaddr.data_size = " << suffix_xaddress<T>::data_size << std::endl;
        std::cerr << "xaddr.num_words = " << suffix_xaddress<T>::num_words << std::endl;
        std::cerr << "sizeof(xaddr)   = " << sizeof(suffix_xaddress<T>) << std::endl;

        float    obj  = 1.2f;
        uint32_t path = 0x12345678;
        suffix_xaddress<T> xaddr(obj,T(path)); std::cerr << "xaddr: "; xaddr.print_to(std::cerr) << std::endl;

    }

}

Y_UTEST(suffix_address)
{
    const short a = 10;
    const float f = 2.0f;

    suffix_address aa(a);
    suffix_address af(f);

    std::cerr << "aa: "; aa.print_to(std::cerr) << std::endl;
    std::cerr << "af: "; af.print_to(std::cerr) << std::endl;

    Y_CHECK( aa.addr() == &a );
    Y_CHECK( af.addr() == &f );

    std::cerr << "a=" << aa.to<short>() << std::endl;
    std::cerr << "f=" << af.to<float>() << std::endl;

    do_xaddr<char>();
    do_xaddr<short>();
    do_xaddr<int>();
    do_xaddr<uint64_t>();


}
Y_UTEST_DONE()

