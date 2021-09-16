
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include <iomanip>
#include "y/mkl/kernel/apk.hpp"
#include "y/yap/lcm.hpp"
#include "y/chemical/sys/flux.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {



        void System::buildOmega()
        {

            Flux::Graph G(strain,primary);
            G.graphViz("flux.dot");
            
        }
    }

}
