
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        const char Reactor:: CLID[] = "Reactor";

        Reactor:: Reactor(Library    &_lib,
                          Equilibria &_eqs) :
        lib(_lib),
        eqs(_eqs),
        N(eqs->size()),
        M(lib->size()),
        dof(0),
        active(M,false),
        K(N,0),
        Gam(N,0),
        xi(N,0),
        dC(M,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        swept(N,false),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            eqs.guess( aliasing::_(active) );
            for(size_t i=M;i>0;--i)
            {
                if(active[i]) aliasing::incr(dof);
            }
            std::cerr << "active=" << active << " // #" << dof << "/" << M << std::endl;
        }

        Reactor:: ~Reactor() throw()
        {
        }

        bool  Reactor:: checkRegular() const throw()
        {
            tao::mmul_rtrn(aliasing::_(W),Phi,Nu,aliasing::_(J));
            return LU::build(aliasing::_(W));
        }


        bool Reactor:: isRegular(const Accessible &C, const double t) throw()
        {
            eqs.compute(aliasing::_(K),aliasing::_(Gam),aliasing::_(Phi),C,t);
            return checkRegular();
        }

        bool Reactor:: isRegular(const Accessible &C) throw()
        {
            eqs.upgrade(K,aliasing::_(Gam),aliasing::_(Phi),C);
            return checkRegular();
        }

        void Reactor:: display_state() const
        {
            std::cerr << "K   = " << K   << std::endl;
            std::cerr << "Gam = " << Gam << std::endl;
            std::cerr << "Phi = " << K   << std::endl;
            std::cerr << "J   = " << J   << std::endl;
        }

    }

}
