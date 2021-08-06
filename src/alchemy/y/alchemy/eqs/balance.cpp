#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {


        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;

            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    aliasing::_(Ctry)[j] = max_of(C[j],0.0);
                }
                else
                {
                    aliasing::_(Ctry)[j] = C[j];
                }
            }
            std::cerr << "Ctry=" << Ctry << std::endl;

            Vector grad(M,0);
            project(grad,Ctry,C);
            tao::neg(grad);
            std::cerr << "grad=" << grad << std::endl;


            return false;
        }

    }

}


