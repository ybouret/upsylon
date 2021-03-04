
#include "y/gfx/pixmaps.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T>
    static inline void do_pixmap()
    {
        std::cerr << "pixmap<" << type_name_of<T>() << ">" << std::endl;
        Y_CHECK(sizeof(bitrow)==sizeof(pixrow<T>));

        std::cerr << std::endl;
    }
}

Y_UTEST(pixmaps)
{
    do_pixmap<uint8_t>();
    do_pixmap<float>();
    do_pixmap<rgb>();
    do_pixmap<rgba>();
    do_pixmap<YUV>();

}
Y_UTEST_DONE()

