
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


        static inline long d2l(const double x) throw()
        {
            return static_cast<long>( floor(x+0.5) );
        }

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
        Ctry(M,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        Psi(M,M),
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
            eqs.guess( aliasing::_(active) );
            for(size_t i=M;i>0;--i)
            {
                if(active[i]) aliasing::incr(dof);
            }
            std::cerr << "active=" << active << " // #" << dof << "/" << M << std::endl;

            {
                matrix<long> aNu3(N,M);
                {
                    matrix<long> aNu2(N,N);
                    {
                        Matrix adj(N,N);
                        {
                            Matrix Nu2(N,N);
                            tao::gram(Nu2,Nu);
                            adjoint(adj,Nu2);
                        }
                        aNu2.assign(adj,d2l);
                    }
                    tao::mmul(aNu3,aNu2,Nu);
                }
                tao::mmul( aliasing::_(Psi),NuT,aNu3);
            }
            std::cerr << "Psi=" << Psi << std::endl;
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
