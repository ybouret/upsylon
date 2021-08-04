#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        size_t Reactor:: invalid(const Accessible &C) throw()
        {
            Flags &flag = aliasing::_(isBad);
            size_t nbad = 0;
            for(size_t j=M;j>0;--j)
            {
                bool &bad = flag[j];
                bad = false;
                if(active[j])
                {
                    const double c = C[j];
                    if(c<0)
                    {
                        ++nbad;
                        bad = true;
                    }
                }

            }
            return nbad;
        }


        bool Reactor:: balance(Addressable &C) throw()
        {
            
            return invalid(C) > 0;
            
        }

    }

}


