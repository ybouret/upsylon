#include "y/yap.hpp"
#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/exception.hpp"
#include <iomanip>

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
        guards(N),
        xi(N,0),
        xiTry(M,as_capacity),
        ok(N,false),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        NuS(M,1),
        aNu2(N,N),
        dNu2(0),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {
            std::cerr << "<Setup " << CLID << ">" << std::endl;
            if(N>M) throw exception("%s detected too many equilibria!",CLID);
            std::cerr << " active = " << active << " // #" << NA << "/" << M << std::endl;

            eqs.verify();


            //__________________________________________________________________
            //
            //
            // initialize Nu and Nu'
            //
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            std::cerr << " Nu     = " << Nu  << std::endl;
            std::cerr << " NuT    = " << NuT << std::endl;

            //__________________________________________________________________
            //
            //
            // checking consistency
            //
            //__________________________________________________________________
            {
                matrix<apz> aNu2_(N,N);
                const apz   dNu2_ = apk::adjoint_gram(aNu2_,Nu);
                apk::convert(aliasing::_(aNu2),aNu2_);
                aliasing::_(dNu2) = dNu2_.cast_to<long>("determinant(Nu2)");
            }
            std::cerr << " dNu2   = " << dNu2 << std::endl;
            if(dNu2<=0) throw exception("%s detected redundant equilibria",CLID);

            

            //__________________________________________________________________
            //
            // Build balancing info:
            // outer loop over species
            //__________________________________________________________________
            std::cerr << "  <Balancing>" << std::endl;
            for(const Species::Node *snode = lib->head(); snode; snode=snode->next)
            {
                const Species &sp = ***snode;
                const size_t   si = sp.indx;

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const accessible<unit_t> &v   = NuT[si];
                size_t                    nok = 0;
                const Equilibrium        *pEq = 0;
                unit_t                    snu = 0;

                //______________________________________________________________
                //
                // inner loop over equilibria
                //______________________________________________________________
                for(const Equilibrium::Node *enode=eqs->head();enode;enode=enode->next)
                {
                    const Equilibrium &eq  = ***enode;
                    const size_t       ei  = eq.indx;
                    const unit_t       cof = v[ei];
                    if(0!=cof)
                    {
                        ++nok;
                        pEq = &eq;
                        snu = cof;
                    }
                }

                if(nok>0)
                {
                    assert(true==active[sp.indx]);
                    assert(snu!=0);
                    assert(pEq!=NULL);
                    
                    if(1==nok)
                    {
                        Guard &guard = aliasing::_(*guards[pEq->indx]);
                        if(snu>0)
                        {
                            //__________________________________________________
                            //
                            // Create a new GEQ
                            //__________________________________________________
                            const Leading leading(*pEq,sp, static_cast<size_t>(snu), Leading::GEQ);
                            leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                            guard.addGEQ(leading);
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // Create a new LEQ
                            //__________________________________________________
                            const Leading leading(*pEq,sp, static_cast<size_t>(-snu), Leading::LEQ);
                            leading.print(std::cerr << "   | ",lib,eqs) << std::endl;
                            guard.addLEQ(leading);
                        }
                    }

                }
                else
                {
                    assert(false==active[sp.indx]);
                }

            }
            std::cerr << "  <Balancing/>" << std::endl;

            std::cerr << " NuS    = " << NuS  << std::endl;
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
