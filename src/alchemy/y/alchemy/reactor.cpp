#include "y/yap.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/exception.hpp"
#include "y/code/textual.hpp"
#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        const char Reactor:: CLID[] = "Reactor";

        Reactor:: Reactor(Library    &_lib,
                          Equilibria &_eqs,
                          const unsigned flags) :
        lib(_lib),
        eqs(_eqs),
        N(eqs->size()),
        M(lib->size()),
        active(M,false),
        NA( eqs.guess( aliasing::_(active) ) ),
        K(N,0),
        Gam(N,0),
        guards(N),
        xi(N,0),
        ok(M,false),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        NuLT(NuT.rows,NuT.cols),
        NuL(Nu.rows,Nu.cols),
        NuST(NuT.rows,NuT.cols),
        NuS(Nu.rows,Nu.cols),
        NL(0),
        NS(0),
        leading(M,as_capacity),
        seeking(M,as_capacity),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        Vs(),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {
            std::cerr << "<Setup " << CLID << ">" << std::endl;
            if(N>M) throw exception("%s detected too many equilibria!",CLID);
            std::cerr << " active = " << active << " // #" << NA << "/" << M << std::endl;

            eqs.verify(flags);
            checkTopology();
            makeBalancing();

            std::cerr << "<Setup " << CLID << "/>" << std::endl;
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
