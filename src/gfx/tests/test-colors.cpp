#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace GFX;

namespace
{
    template <typename T, typename U>
    static inline void  CheckFrom(const U &u)
    {
        std::cerr << "Convert<" << type_name_of<T>() << ">::From<" << type_name_of<U>() << ">" << std::endl;
        std::cerr << u << " -> " << Convert<T>::template From<U>(u) << std::endl;
    }
}

Y_UTEST(colors)
{
    const float   f = 0.2f;
    const uint8_t u = 'A';
    const rgb     c(10,20,30);
    
    CheckFrom<float,float>(f);
    CheckFrom<uint8_t,uint8_t>(u);
    CheckFrom<uint8_t,float>(f);
    CheckFrom<uint8_t,rgb>(c);

    
}
Y_UTEST_DONE()

