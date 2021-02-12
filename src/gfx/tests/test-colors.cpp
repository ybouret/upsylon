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
    
    template <typename T>
    void CheckAll(const T &value)
    {
        Check<T,uint8_t>(value);
        Check<T,float>(value);
        Check<T,rgb>(value);
        Check<T,rgba>(value);
        Check<T,YUV>(value);
    }
}

Y_UTEST(colors)
{
    //const float   f = 0.2f;
    
    {
        const uint8_t     u = 'A';
        CheckAll<uint8_t>(u);
    }
    
    {
        const float     f = 0.22f;
        //CheckAll<float>(f);
    }
    
    {
        const rgb     c(10,20,30);
        //CheckAll<rgb>(c);
    }
    
    
    
        
    
}
Y_UTEST_DONE()

