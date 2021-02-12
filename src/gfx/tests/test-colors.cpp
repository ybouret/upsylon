#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace GFX;

namespace
{
    template <typename T, typename U>
    static inline void  Check(const T &value)
    {
        std::cerr << "Convert<" << type_name_of<T>() << ">::From<" << type_name_of<U>() << ">" << std::endl;
        std::cerr << value << " -> " << Convert<T>::template To<U>(value) << std::endl;
    }
}

Y_UTEST(colors)
{
    //const float   f = 0.2f;
    const rgb     c(10,20,30);
    
    {
        const uint8_t u = 'A';
        Check<uint8_t,uint8_t>(u);
        Check<uint8_t,float>(u);
        Check<uint8_t,rgb>(u);
        Check<uint8_t,rgba>(u);
    }
    
    {
        const float f = 0.22f;
        Check<float,uint8_t>(f);
        Check<float,float>(f);
        Check<float,rgb>(f);
        Check<float,rgba>(f);
    }
    
        
    
}
Y_UTEST_DONE()

