
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
        K(N,0),
        Gam(N,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        Phi(Nu.rows,Nu.cols),
        W(N,N),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            

        }

        Reactor:: ~Reactor() throw()
        {
        }

        bool Reactor:: initialize(const Accessible &C, const double t) throw()
        {
            eqs.compute(aliasing::_(K),aliasing::_(Gam),aliasing::_(Phi),C,t);
            std::cerr << "K=" << K << std::endl;
            std::cerr << "Gam=" << Gam << std::endl;
            std::cerr << "Phi=" << Phi << std::endl;
            tao::mmul_rtrn(aliasing::_(W),Phi,Nu);
            std::cerr << "PhiNuT=" << W << std::endl;
            return LU::build(aliasing::_(W));
        }


    }

}
