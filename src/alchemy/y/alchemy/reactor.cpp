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
        xi(N,0),
        aux1(N,0),
        aux2(N,0),
        Caux(M,0),
        dC(M,0),
        Ctry(M,0),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        Nu1(NuT.rows,false),
        aNu2(N,N),
        dNu2(0),
        Phi(Nu.rows,Nu.cols),
        J(N,N),
        W(N,N),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {

            if(N>M) throw exception("%s detected too many equilibria!",CLID);
            std::cerr << "active=" << active << " // #" << NA << "/" << M << std::endl;

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            eqs.fill( aliasing::_(Nu) );
            aliasing::_(NuT).assign_transpose(Nu);
            {
                matrix<apz> aNu2_(N,N);
                const apz   dNu2_ = apk::adjoint_gram(aNu2_,Nu);
                apk::convert(aliasing::_(aNu2),aNu2_);
                aliasing::_(dNu2) = dNu2_.cast_to<long>("determinant(Nu2)");
            }
            std::cerr << "aNu2=" << aNu2 << std::endl;
            std::cerr << "dNu2=" << dNu2 << std::endl;
            if(dNu2<=0) throw exception("%s detected redundant equilibria",CLID);

            for(size_t j=Nu1.size();j>0;--j)
            {
                const accessible<long> &NuTj = NuT[j];
                size_t                  neqz = 0;
                for(size_t i=N;i>0;--i)
                {
                    if(NuTj[i]!=0) ++neqz;
                }
                if(1==neqz) aliasing::_(Nu1[j]) = true;
            }
            std::cerr << "Nu1=" << Nu1 << std::endl;
            
#if 0
            iMatrix dXi(M,M);
            {
                iMatrix Nu3(N,M);
                tao::mmul(Nu3,aNu2,Nu);
                std::cerr << "Nu3=" << Nu3 << std::endl;
                tao::mmul(dXi,NuT,Nu3);
            }
            std::cerr << "Xi=" << dXi << "/" << dNu2 << std::endl;
            
            iMatrix dOm(M,M);
            for(size_t i=M;i>0;--i)
            {
                for(size_t j=M;j>i;--j)
                {
                    dOm[i][j] = -dXi[i][j];
                }
                dOm[i][i] = dNu2 - dXi[i][i];
                for(size_t j=i-1;j>0;--j)
                {
                    dOm[i][j] = -dXi[i][j];
                }
            }
            std::cerr << "Om=" << dOm << "/" << dNu2 << std::endl;
#endif
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

        void Reactor:: project(Addressable &delta, const Accessible &C) throw()
        {
            tao::mul(aliasing::_(aux1),Nu,C);
            tao::mul(aliasing::_(aux2),aNu2,aux1);
            tao::mul(delta,NuT,aux2);
            tao::divset(delta,dNu2);
        }

        void Reactor:: project(Addressable &delta, const Accessible &C, const Accessible &C0) throw()
        {
            tao::sub(aliasing::_(Caux),C0,C);
            project(delta,Caux);
        }

        void Reactor:: complete()
        {
            iMatrix F(M,M);
            Flags   used(M,false);

            std::cerr << "1:" << N << " => Nu" << std::endl;
            for(size_t i=N;i>0;--i)
            {
                tao::set(F[i],Nu[i]);
                used[i] = true;
            }

            size_t count = N;

            {
                for(const Species::Node *sp = lib->head();sp;sp=sp->next)
                {
                    const Species &s = ***sp;
                    if(s.active) continue;
                    const size_t j = s.indx;
                    F[j][j] = 1;
                    used[j] = true;
                    ++count;
                }
            }

            std::cerr << "F       = " << F       << std::endl;
            std::cerr << "used    = " << used    << std::endl;
            std::cerr << "missing = " << M-count << std::endl;
        }

    }

}
