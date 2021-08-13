
#include "y/alchemy/guard.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

#define Y_ALCHEMY_GUARD(CLS) case CLS: return #CLS

        const char * Guard:: ClassText(const Class c) throw()
        {
            switch(c)
            {
                    Y_ALCHEMY_GUARD(HasNoBound);
                    Y_ALCHEMY_GUARD(HasOnlyLEQ);
                    Y_ALCHEMY_GUARD(HasOnlyGEQ);
                    Y_ALCHEMY_GUARD(IsBothWays);
            }
            return "???";
        }

        const char * Guard:: classText() const throw()
        {
            return ClassText(cls);
        }


        Guard:: ~Guard() throw()
        {
        }

        Guard:: Guard() throw() :
        leq(), geq(), cls(HasNoBound)
        {
        }

        Guard:: Array:: ~Array() throw()
        {
        }

        Guard::Array:: Array(size_t n) :
        Array_(n,as_capacity)
        {
            while(n-- > 0)
            {
                const Guard::Pointer p = new Guard();
                push_back_(p);
            }
        }

        void Guard:: addLEQ(const Leading &leading)
        {
            aliasing::_(leq).push_back(leading);
            update();
        }

        void Guard:: addGEQ(const Leading &leading)
        {
            aliasing::_(geq).push_back(leading);
            update();
        }

        void Guard:: update() throw()
        {
            if(leq.size()>0)
            {
                if(geq.size()>0)
                {
                    aliasing::_(cls) = IsBothWays;
                }
                else
                {
                    assert(geq.size()<=0);
                    aliasing::_(cls) = HasOnlyLEQ;
                }
            }
            else
            {
                if(geq.size()>0)
                {
                    assert(leq.size()<=0);
                    aliasing::_(cls) = HasOnlyGEQ;
                }
                else
                {
                    assert(geq.size()<=0);
                    assert(leq.size()<=0);
                    aliasing::_(cls) = HasNoBound;
                }
            }
        }



        //! getMax = min of leq, leq.size()>0
        const Leading & Guard:: xiMax(const Accessible &C) const throw()
        {
            assert(leq.size()>0);
            assert(HasOnlyLEQ==cls||IsBothWays==cls);
            const Leading *opt = &leq[1];
            for(size_t i=leq.size();i>1;--i)
            {
                const Leading *tmp = &leq[i];
                const double xi_opt = C[opt->sp.indx]/opt->nu;
                const double xi_tmp = C[tmp->sp.indx]/tmp->nu;
                if(xi_tmp<xi_opt)
                {
                    opt = tmp;
                }
            }
            return *opt;
        }


        //! getMin = max of geq, geq.size()>0
        const Leading & Guard:: xiMin(const Accessible &C) const throw()
        {
            assert(geq.size()>0);
            assert(HasOnlyGEQ==cls||IsBothWays==cls);
            const Leading *opt = &geq[1];
            for(size_t i=geq.size();i>1;--i)
            {
                const Leading *tmp    = &geq[i];
                const double   xi_opt = -C[opt->sp.indx]/opt->nu;
                const double   xi_tmp = -C[tmp->sp.indx]/tmp->nu;
                if(xi_tmp>xi_opt)
                {
                    opt = tmp;
                }
            }
            return *opt;
        }


        void Guard:: solve(Addressable &C, const iMatrix &NuT, Addressable &xi ) const throw()
        {
            std::cerr << "  " << classText() << std::endl;
            switch(cls)
            {
                case HasNoBound: return;

                case HasOnlyGEQ: {
                    const Leading &lmin = xiMin(C);
                    const double   xmin  =-C[lmin.sp.indx]/lmin.nu;
                    std::cerr << "  xmin=" << xmin << std::endl;
                    if(xmin>0)
                    {
                        tao::ld(xi,0);
                        xi[lmin.eq.indx] = xmin;
                        tao::mul_add(C,NuT,xi);
                        C[lmin.sp.indx]  = 0;
                    }
                } break;

                case HasOnlyLEQ:
                    break;

                case IsBothWays:
                    break;
            }
        }

    }

}
