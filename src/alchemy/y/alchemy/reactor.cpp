#include "y/yap.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/exception.hpp"

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
        active(M,false),
        NA( eqs.guess( aliasing::_(active) ) ),
        K(N,0),
        Gam(N,0),
        xi(N,0),
        dC(M,0),
        Ctry(M,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {

            if(N>M) throw exception("%s detected too many equilibria!",CLID);

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            std::cerr << "active=" << active << " // #" << NA << "/" << M << std::endl;

            matrix<apq> Nu2(N,N);
            tao::gram(Nu2,Nu);
            std::cerr << "Nu2  = " << Nu2 << std::endl;
            matrix<apq> aNu2(N,N);
            adjoint(aNu2,Nu2);
            std::cerr << "aNu2 = " << aNu2 << std::endl;
            const apq dNu2 = __determinant(Nu2);
            std::cerr << "dNu2 = " << dNu2 << std::endl;
            if(dNu2<=0) throw exception("%s has redundant equilibria!",CLID);

        }

        Reactor:: ~Reactor() throw()
        {
        }


        void Reactor:: displayState() const
        {
            std::cerr << "K   = " << K   << std::endl;
            std::cerr << "Gam = " << Gam << std::endl;
            std::cerr << "Phi = " << K   << std::endl;
            std::cerr << "J   = " << J   << std::endl;
        }

        bool Reactor:: isValid(const Accessible &C) const
        {
            bool ans = true;
            for(Library::const_iterator it=lib->begin();it!=lib->end();++it)
            {
                const Species &sp = **it;
                if(!active[sp.indx]) continue;;
                if(C[sp.indx]<0)
                {
                    ans = false;
                    std::cerr << sp << "=" << C[sp.indx] << "<0 !" << std::endl;
                }

            }
            return ans;
        }


    }

}
