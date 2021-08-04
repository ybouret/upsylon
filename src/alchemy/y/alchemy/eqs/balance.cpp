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
            std::cerr << "bad = " << isBad << " // #" << nbad << std::endl;
            return nbad;
        }


        bool Reactor:: balance(Addressable &C) throw()
        {
            
            if(invalid(C)>0)
            {
                for(size_t i=1;i<=N;++i)
                {
                    const accessible<long> &nu = Nu[i];
                    std::cerr << "nu[" << i << "]=" << nu << std::endl;
                }
                return false;
            }
            else
            {
                return true;
            }
            
        }

    }

}


