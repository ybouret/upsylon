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
                aliasing::_(Ctry[j]) = 0;
                if(active[j]&&C[j]<0)
                {
                    aliasing::_(Ctry[j]) = -C[j];
                    ++nbad;
                }
            }
            std::cerr << "Cbad=" << Ctry << std::endl;
            return false;
        }

    }

}


