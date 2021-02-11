
#include "y/gfx/pixmap.hpp"
#include "y/gfx/async/broker.hpp"
#include "y/concurrent/scheme/nexus.hpp"

#include "y/utest/run.hpp"
#include "../../main/tests/support.hpp"

#include "y/utest/timings.hpp"
#include <cmath>

#include "y/gfx/async/ops/gradient.hpp"


using namespace upsylon;
using namespace GFX;

namespace
{
    template <typename T>
    T id(const T &x)
    {
        return (x);
    }
    
    
}

Y_UTEST(grad)
{
    Async::SharedServer seq = new concurrent::sequential_server(); Y_CHECK((**seq).size()==1);
    Async::SharedServer par = new concurrent::nexus();
    
    {
        unit_t        w = 200;
        unit_t        h = 300;
        Pixmap<float> pf(w,h);
        Async::SharedEngine seqEng = new Async::Engine(pf,(**seq).size());
        Async::SharedEngine parEng = new Async::Engine(pf,(**par).size());
        
    }
    
}
Y_UTEST_DONE()

