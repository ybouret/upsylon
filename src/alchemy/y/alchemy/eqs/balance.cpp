#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        static inline void displayCoef(const size_t coef)
        {
            if(coef>1) std::cerr << coef << ' ';
        }




        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    if(sp.active)
                    {
                        const size_t j = sp.indx;
                        lib.print(std::cerr,sp) << " = " << C[j] << " -> " << NuT[j] << " ";
                        const Balancing::Info &cond = Cond[j];
                        switch(cond.type)
                        {
                            case Balancing::None: std::cerr << "None";
                                assert( die("never get here") );
                                break;

                            case Balancing::Free: std::cerr << "Free: ";
                                for(size_t i=1;i<=N;++i)
                                {
                                    const long nu = NuT[j][i];
                                    if(nu<0)
                                    {
                                        if(nu < -1) std::cerr << nu << ' ';
                                        else        std::cerr << '-';
                                    }
                                    else
                                    {
                                        if(nu>0)
                                        {
                                            std::cerr << '+';
                                            if(nu>1) std::cerr << nu;
                                        }
                                        else
                                        {
                                            // nothing
                                        }
                                    }
                                    std::cerr << "xi_" << eqs(i).name;
                                }
                                std::cerr << " >= " << C[j];
                                break;

                            case Balancing::LEQT: std::cerr << "LEQT: ";
                                assert(cond.coef>0);
                                displayCoef(cond.coef);
                                std::cerr << "xi_" << eqs(cond.indx).name << " <= " << C[j];
                                break;

                            case Balancing::GEQT: std::cerr << "GEQT: ";
                                assert(cond.coef>0);
                                displayCoef(cond.coef);
                                std::cerr << "xi_" << eqs(cond.indx).name << " >= " << -C[j];
                                break;
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


