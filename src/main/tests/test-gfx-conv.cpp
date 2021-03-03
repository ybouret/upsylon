#include "y/graphic/color/rgba-to-type.hpp"
#include "y/graphic/color/type-to-rgba.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T>
    void do_check( const T &data )
    {
        std::cerr << "checking with <" << type_name_of<T>() << ">" << std::endl;
        get_rgba<T> io_get;

        rgba c = io_get(&data);
        std::cerr << data << " -> " << c << std::endl;
    }
}

Y_UTEST(gfx_conv)
{
    {
        uint8_t u=100;
        do_check(u);
    }

}
Y_UTEST_DONE()


