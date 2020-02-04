#include "y/type/divs.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {

}

Y_UTEST(divs)
{
    std::cerr << type_spec_of<int>()       << std::endl;
    std::cerr << type_spec_of<long>()      << std::endl;
    std::cerr << type_spec_of<long long>() << std::endl;
    std::cerr << type_spec_of<unit_t>() << std::endl;

    typedef idiv::of<unit_t> idiv_t;

    idiv_t::type l = idiv_t::_(5,4);
    std::cerr << "l.quot=" << l.quot << ", l.rem=" << l.rem << std::endl;


}
Y_UTEST_DONE()

