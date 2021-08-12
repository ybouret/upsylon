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
        sentries(N),
        xi(N),
        Cpsi(M,0),
        Xpsi(N,0),
        Xtry(N,0),
        Ctry(M,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        NuS(M,1),
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
            //
            // building balance info
            //
            //__________________________________________________________________
            for(size_t sp=NuT.rows;sp>0;--sp)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const accessible<long> &v   = NuT[sp];
                size_t                  nok = 0;
                size_t                  eq  = 0;
                long                    nu  = 0;
                double                  sum = 0;

                //______________________________________________________________
                //
                // loop to count active equilibria changing species
                //______________________________________________________________
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

                //______________________________________________________________
                //
                // check active species
                //______________________________________________________________
                if(nok>0)
                {
                    assert(true==active[sp]);
                    aliasing::_(NuS[sp]) = sum;
                    if(1==nok)
                    {
                        assert(nu!=0);
                        if(nu>0)
                        {
                            //std::cerr << "New GEQ" << std::endl;
                            const Primary primary(eq,sp,static_cast<size_t>(nu));
                            aliasing::_(sentries)[eq]->addGEQ(primary);
                        }
                        else
                        {
                            //std::cerr << "New LEQ" << std::endl;
                            const Primary primary(eq,sp,static_cast<size_t>(-nu));
                            aliasing::_(sentries)[eq]->addLEQ(primary);
                        }
                    }
                }
                else
                {
                    assert(false==active[sp]);
                }
            }
            std::cerr << " NuS    = " << NuS  << std::endl;

            std::cerr << "  <Sentries>" << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                const Sentry      &sentry = *sentries[i];
                const Equilibrium &eq     = eqs(i);
                eqs.print(std::cerr << "    ", eq) << " : " << sentry.typeText() << std::endl;
                for(size_t j=sentry.leq.size();j>0;--j)
                {
                    const Primary &p = sentry.leq[j];
                    std::cerr << "     | ";
                    if(p.nu>1)
                        std::cerr << std::setw(2) << p.nu << "*";
                    std::cerr << Primary::Prefix;
                    eqs.print(std::cerr,eq) << " <=  " << lib(p.sp) << std::endl;
                }
                for(size_t j=sentry.geq.size();j>0;--j)
                {
                    const Primary &p = sentry.geq[j];
                    std::cerr << "     | ";
                    if(p.nu>1)
                        std::cerr << std::setw(2) << p.nu << "*";
                    std::cerr << Primary::Prefix;
                    eqs.print(std::cerr,eq) << " >= -" << lib(p.sp) << std::endl;
                }


                eqs.print(std::cerr << "    ", eqs(i)) << std::endl;
                if(i<N)
                    std::cerr << std::endl;
            }
            std::cerr << "  <Sentries/>" << std::endl;
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
