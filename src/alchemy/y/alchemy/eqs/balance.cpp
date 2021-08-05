#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        bool Reactor:: balance(Addressable &C) throw()
        {
            vector<size_t> ibad;

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
                    ibad.push_back(j);
                }
            }
            std::cerr << "nbad=" << nbad << std::endl;
            std::cerr << "cbad=" << dC   << std::endl;
            std::cerr << "ibad=" << ibad << std::endl;



            return false;
        }

    }

}


