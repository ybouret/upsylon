
#include "y/gfx/stack.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T>
    static inline void do_stack()
    {
        const size_t n = 2 + alea.leq(10);
        const unit_t w = 1 + alea.leq(100);
        const unit_t h = 1 + alea.leq(100);
        
        stack<T> s(n,w,h);
        std::cerr << "#s=" << s.size() << std::endl;
        
        const string filename = "stack-" + type_name_of<T>() + ".tif";
        s.save_tiff(filename,0,s.size());
        
    }
}

Y_UTEST(stack)
{
    do_stack<uint8_t>();
    do_stack<float>();
}
Y_UTEST_DONE()

