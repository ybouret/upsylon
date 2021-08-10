#include "y/yap.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
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
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        NuS(M,1),
        cond(NuT.rows,as_capacity),
        aNu2(N,N),
        dNu2(0),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        Csqr(M,as_capacity),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {
            std::cerr << "<Setup " << CLID << ">" << std::endl;
            if(N>M) throw exception("%s detected too many equilibria!",CLID);
            std::cerr << " active = " << active << " // #" << NA << "/" << M << std::endl;

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            std::cerr << " Nu    = " << Nu  << std::endl;
            std::cerr << " NuT   = " << NuT << std::endl;

            //__________________________________________________________________
            //
            // checking consistency
            //__________________________________________________________________
            {
                matrix<apz> aNu2_(N,N);
                const apz   dNu2_ = apk::adjoint_gram(aNu2_,Nu);
                apk::convert(aliasing::_(aNu2),aNu2_);
                aliasing::_(dNu2) = dNu2_.cast_to<long>("determinant(Nu2)");
            }
            std::cerr << " dNu2  = " << dNu2 << std::endl;
            if(dNu2<=0) throw exception("%s detected redundant equilibria",CLID);

            //__________________________________________________________________
            //
            // building balance info
            //__________________________________________________________________
            for(size_t sp=NuT.rows;sp>0;--sp)
            {
                const accessible<long> &v   = NuT[sp];
                size_t                  nok = 0;
                size_t                  eq  = 0;
                long                    nu  = 0;
                double                  sum = 0;

                // loop to count active equilibria changing species
                for(size_t i=N;i>0;--i)
                {
                    const long cof = v[i];
                    if(0!=cof)
                    {
                        nu   = cof;
                        eq   = i;
                        sum += abs_of(nu);
                        ++nok;
                    }
                }

                // check active species
                if(nok>0)
                {
                    assert( active[sp] );
                    aliasing::_(NuS[sp]) = sum;
                    if(1==nok)
                    {
                        assert(nu!=0);
                        const Condition cc(eq,sp,nu,lib,eqs);
                        aliasing::_(cond).push_back_(cc);
                    }
                }
                else
                {
                    assert( !active[sp] );
                }
            }
            
            std::cerr << " NuS   = " << NuS  << std::endl;
            std::cerr << " cond  : " << cond << std::endl;
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
