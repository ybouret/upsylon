#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        bool Reactor:: balance(Addressable &C) throw()
        {

            size_t nbad = 0;
            for(size_t j=M;j>0;--j)
            {
                const double c = C[j];
                double      &d = aliasing::_(dC[j]);
                d=0;
                if(active[j] && c<0)
                {
                    d = -c;
                    ++nbad;
                }
            }
            std::cerr << "nbad=" << nbad << std::endl;
            std::cerr << "cbad=" << dC   << std::endl;

            return false;
        }

    }

}


