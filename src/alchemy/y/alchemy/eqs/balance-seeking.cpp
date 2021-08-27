
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
                lib.display(std::cerr,C) << std::endl;
            }

            if(NS>0)
            {
                Vector         Cs(NS,0);
                Vector         Xs(N,0);
                Addressable   &Xi = aliasing::_(xi);
                Y_ALCHEM_PRINTLN("Vs="<<Vs);

                matrix<double> Vm(NS,N);
                matrix<double> tV(N,NS);
                matrix<double> V2(NS,NS);

                while(true)
                {
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

                        bool clipped = false;
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
                                    const double   xmin = -C[lmin.sp.indx] / lmin.nu;
                                    if(x>=xmin)
                                    {
                                        std::cerr << "  |_valid" << std::endl;
                                    }
                                    else
                                    {
                                        tao::ld(Xi,0);
                                        Xi[i] = xmin;
                                        tao::mul_add(C,NuT,Xi);
                                        C[lmin.sp.indx] = 0;
                                        aliasing::_(Vs).ld_col(i,0);
                                        clipped = true;
                                    }

                                } break;

                                case Guard::HasOnlyLEQ:
                                {
                                    const Leading &lmax = g.xiMax(C); assert( &lmax.eq == &_ );
                                    const double   xmax = C[lmax.sp.indx] / lmax.nu;
                                    if(x<=xmax)
                                    {
                                        std::cerr << "  |_valid" << std::endl;
                                    }
                                    else
                                    {
                                        tao::ld(Xi,0);
                                        Xi[i] = xmax;
                                        tao::mul_add(C,NuT,Xi);
                                        C[lmax.sp.indx] = 0;
                                        aliasing::_(Vs).ld_col(i,0);
                                        clipped = true;
                                    }
                                }
                                    break;

                                case Guard::IsBothWays:
                                {
                                    const Leading &lmin = g.xiMin(C); assert( &lmin.eq == &_ );
                                    const double   xmin = -C[lmin.sp.indx] / lmin.nu;
                                    const Leading &lmax = g.xiMax(C); assert( &lmax.eq == &_ );
                                    const double   xmax = C[lmax.sp.indx] / lmax.nu;
                                    std::cerr << "  [" << xmin << ":" << xmax << "]" << std::endl;

                                    if(x<xmin)
                                    {
                                        tao::ld(Xi,0);
                                        Xi[i] = xmin;
                                        tao::mul_add(C,NuT,Xi);
                                        C[lmin.sp.indx] = 0;
                                        aliasing::_(Vs).ld_col(i,0);
                                        clipped = true;

                                    }

                                    if(x>xmax)
                                    {
                                        tao::ld(Xi,0);
                                        Xi[i] = xmax;
                                        tao::mul_add(C,NuT,Xi);
                                        C[lmax.sp.indx] = 0;
                                        aliasing::_(Vs).ld_col(i,0);
                                        clipped = true;
                                    }


                                }   break;
                            }
                        }

                        lib.display(std::cerr,C) << std::endl;

                        std::cerr << "Vs=" << Vs << std::endl;
                        std::cerr << "clipped=" << clipped << std::endl;
                        if(clipped)
                            continue;

                        // found
                        tao::mul_add(C,NuT,Xs);
                        for(size_t j=M;j>0;--j)
                        {
                            if(active[j]&&C[j]<=0) C[j]=0;
                        }

                        break;

                    }
                    else
                    {
                        goto DONE;
                    }

                }

            }

        DONE:
            if(Verbosity)
            {
                std::cerr << "  ==> [" <<textual::boolean(balanced) << "] <==" << std::endl;
                lib.display(std::cerr,C) << std::endl;
                std::cerr << "<Balanced Seeking/>" << std::endl;
            }

            return balanced;
        }
    }

}

