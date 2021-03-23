
#include "y/gfx/stack.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/gfx/color/random.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T>
    static inline void do_stack(const string id)
    {
        const size_t n = 2 + alea.leq(10);
        const unit_t w = 1 + alea.leq(100);
        const unit_t h = 1 + alea.leq(100);
        
        stack<T> s(n,w,h);
        std::cerr << "#s=" << s.size() << std::endl;
        
        for(size_t i=0;i<n;++i)
        {
            pixmap<T> &pxm = s[i];
            for(unit_t y=0;y<h;++y)
            {
                for(unit_t x=0;x<w;++x)
                {
                    pxm[y][x] = random_color::get<T>(alea);
                }
            }
        }
        
        const string filename = "stack-" + id + ".tif";
        s.save_tiff(filename,0,s.size());
        
    }
}

Y_UTEST(stack)
{
    do_stack<uint8_t>("byte");
    do_stack<float>("float");
    do_stack<rgba>("rgba");
    
    if(argc>1)
    {
        const string filename = argv[1];
        
        stack<float> sgs(filename);
        std::cerr << "stack: " << sgs.size() << std::endl;
    }
}
Y_UTEST_DONE()

