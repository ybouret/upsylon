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
            if(N>0)
            {
                assert(NA>0);
                for(Library::const_iterator it=lib->begin();it!=lib->end();++it)
                {
                    const Species &sp = **it;
                    if(sp.active)
                    {
                        const size_t j = sp.indx;
                        lib.print(std::cerr,sp) << " = " << C[j] << " -> " << NuT[j] << " ";;
                        const Balancing::Info &cond = Cond[j];
                        switch(cond.type)
                        {
                            case Balancing::None: std::cerr << "None"; break;
                            case Balancing::Free: std::cerr << "Free"; break;
                            case Balancing::LEQT: std::cerr << "LEQT"; break;
                            case Balancing::GEQT: std::cerr << "GEQT"; break;
                        }
                        std::cerr << std::endl;
                    }
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


