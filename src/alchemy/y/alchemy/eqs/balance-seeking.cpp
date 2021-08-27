
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/type/utils.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        static inline double u2d(const unit_t u)
        {
            return double(u);
        }

        size_t Reactor:: seekingSpecies(Addressable &Cbad, const Accessible &C) const throw()
        {
            assert(NS==Cbad.size());
            size_t nbad = 0;
            for(size_t j=NS;j>0;--j)
            {
                const double Cj = C[ seeking[j]->indx ];
                if(Cj<=0)
                {
                    Cbad[j] = -Cj;
                    ++nbad;
                }
                else
                {
                    Cbad[j] = 0;
                }
            }
            return nbad;
        }


        bool Reactor:: balanceSeeking(Addressable &C)
        {
            bool balanced = true;

            if(Verbosity)
            {
                std::cerr << "<Balance Seeking>" << std::endl;
                showConditions(std::cerr,C);
            }

            if(NS>0)
            {
                Vector         Cs(NS,0);
                Vector         Xs(N,0);
                Y_ALCHEM_PRINTLN("Vs="<<Vs);

                matrix<double> Vm(NS,N);
                matrix<double> tV(N,NS);
                matrix<double> V2(NS,NS);

                Vm.assign(Vs,u2d);
                tV.assign_transpose(Vm);
                tao::gram(V2,Vm);
                Y_ALCHEM_PRINTLN("V2="<<V2);
                if(!LU::build(V2))
                {
                    Y_ALCHEM_PRINTLN("  singular seeking matrix");
                    balanced = false;
                    goto DONE;
                }
                const size_t nb = seekingSpecies(Cs,C);
                Y_ALCHEM_PRINTLN("Cs="<<Cs);
                if(nb>0)
                {
                    LU::solve(V2,Cs);
                    tao::mul(Xs,tV,Cs);
                    Y_ALCHEM_PRINTLN("Xs="<<Xs);

                    
                    for(const Equilibrium::Node *node=eqs->head();node;node=node->next)
                    {
                        const Equilibrium &_ = ***node;
                        const size_t       i = _.indx;
                        const Guard       &g = *guards[i];
                        double            &x = Xs[i];
                        eqs.print(std::cerr << "  ",_) << ' ' << g.classText() << '/' << x << std::endl;
                        switch(g.cls)
                        {
                            case Guard::HasNoBound:
                                break;

                            case Guard::HasOnlyGEQ: {
                                const Leading &lmin = g.xiMin(C); assert( &lmin.eq == &_ );
                            } break;

                            case Guard::HasOnlyLEQ:
                                break;

                            case Guard::IsBothWays:
                                break;
                        }
                    }


                }
                else
                {
                    goto DONE;
                }

            }

        DONE:
            if(Verbosity)
            {
                std::cerr << "  ==> [" <<textual::boolean(balanced) << "] <==" << std::endl;
                std::cerr << "<Balanced Seeking/>" << std::endl;
            }

            return balanced;
        }
    }

}

