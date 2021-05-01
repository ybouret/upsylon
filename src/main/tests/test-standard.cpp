#include "y/type/standard.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;


Y_UTEST(standard)
{
    std::cerr << rtti::of<int>()       << std::endl;
    std::cerr << rtti::of<long>()      << std::endl;
    std::cerr << rtti::of<long long>() << std::endl;
    std::cerr << rtti::of<unit_t>()    << std::endl;

    {
        typedef core::standard<unit_t> ustd;

        ustd::div_type l = ustd::div_call(5,4);
        std::cerr << "l.quot=" << l.quot << ", l.rem=" << l.rem << std::endl;
    }

    {
        typedef standard<int> istd;
        const istd::div_type l = istd::api::div_call(5,4);
        std::cerr << "l.quot=" << l.quot << ", l.rem=" << l.rem << std::endl;
    }

    {
        typedef standard<long> istd;
        const istd::div_type l = istd::api::div_call(5,4);
        std::cerr << "l.quot=" << l.quot << ", l.rem=" << l.rem << std::endl;
    }

    {
        typedef standard<long long> istd;
        const istd::div_type l = istd::api::div_call(5,4);
        std::cerr << "l.quot=" << l.quot << ", l.rem=" << l.rem << std::endl;
    }

}
Y_UTEST_DONE()

