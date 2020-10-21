#include "y/aqua/boot.hpp"


#include <iomanip>


namespace upsylon {

    namespace Aqua
    {

        void Boot:: setup() throw()
        {
            keep << aliasing::_(R);
            keep << aliasing::_(tR);
            keep << aliasing::_(pL);
            keep << aliasing::_(S);
            keep << aliasing::_(tS);
            keep << aliasing::_(pS);
        }

        Boot:: ~Boot() throw()
        {
        }

        void Boot:: quit() throw()
        {
            keep.release_all();
            aliasing::_(dL) = 0;
            aliasing::_(dS) = 0;
        }

        Constraint & Boot:: operator()( const double value )
        {
            return * push_back( new Constraint(value) );
        }

        void Boot:: conserve(const double C0, const Species &a)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
        }


        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c, const Species &d)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
            cc.add(d,1);
        }


        std::ostream & operator<<(std::ostream &os, const Boot &b)
        {
            os << "<boot #" << b.size << ">" << std::endl;
            for(const Constraint *cc=b.head;cc;cc=cc->next)
            {
                os << ' ' << ' ' << *cc << std::endl;
            }
            os <<  "<boot/>";
            return os;
        }

    }

}

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/simplify.hpp"
#include "y/exception.hpp"
#include "y/aqua/engine.hpp"

namespace upsylon {

    namespace Aqua
    {

        using namespace mkl;

        void Boot:: fill(addressable<double> &Lambda) const throw()
        {
            assert(Lambda.size() == size);
            size_t i=1;
            for(const Constraint *cc=head;cc;cc=cc->next,++i)
            {
                Lambda[i] = cc->value;
            }
        }

        void  Boot:: init(Library          &lib,
                          const Equilibria &eqs)
        {
            static const char fn[] = "Aqua::Boot::init: ";
            quit();
            try
            {
                //--------------------------------------------------------------
                // parameters for R
                //--------------------------------------------------------------
                const size_t M  = lib.entries();
                const size_t N  = eqs.entries();
                const size_t Nc = size;
                if(N+Nc!=M)
                {
                    throw exception("%s: #constraint=%lu + #equilibrium=%lu != #species=%lu",fn, (unsigned long)Nc, (unsigned long)N, (unsigned long)M);
                }

                //--------------------------------------------------------------
                // allocated
                //--------------------------------------------------------------
                aliasing::_(R).  make(Nc,M);
                aliasing::_(tR). make(M,Nc);
                aliasing::_(pL). make(M,Nc);
                aliasing::_(S).  make(N,M);
                aliasing::_(tS). make(M,N);
                aliasing::_(pS). make(M,M);

                //--------------------------------------------------------------
                // fill R
                //--------------------------------------------------------------
                lib.buildIndices();
                {
                    size_t i=1;
                    for(const Constraint *cc=head;cc;cc=cc->next,++i)
                    {
                        cc->fill( aliasing::_(R[i]) );
                    }
                }
                aliasing::_(tR).assign_transpose(R);

                //--------------------------------------------------------------
                // compute Lambda => C matrix, with scaling
                //--------------------------------------------------------------
                {
                    iMatrix R2(Nc,Nc);
                    tao::gram(R2,R);
                    aliasing::_(dL) = ideterminant(R2);
                    if(0==dL)
                    {
                        throw exception("%ssingular set of constraints",fn);
                    }
                    iMatrix aR2(Nc,Nc);
                    iadjoint(aR2,R2);
                    tao::mmul(aliasing::_(pL),tR,aR2);
                    (void) simplify<Int>::on( aliasing::_(pL), aliasing::_(dL) );
                }

                //--------------------------------------------------------------
                // find supplementary space
                //--------------------------------------------------------------
                {
                    iMatrix F(M,M);
                    for(size_t i=Nc;i>0;--i)
                    {
                        tao::set(F[i],R[i]);
                    }
                    {
                        size_t i=1+Nc;
                        for(Equilibria::const_iterator it=eqs.begin();it!=eqs.end();++it,++i)
                        {
                            (**it).fillNu(F[i]);
                        }
                    }
                    if(!GramSchmidt::iOrtho(F))
                    {
                        throw exception("%sinvalid set of constraints",fn);
                    }
                    for(size_t i=N;i>0;--i)
                    {
                        tao::set(aliasing::_(S)[i],F[i+Nc]);
                    }
                }

                //--------------------------------------------------------------
                // compute projection matrix for balancing
                //--------------------------------------------------------------
                aliasing::_(tS).assign_transpose(S);
                aliasing::_(dS) = Engine::Project(aliasing::_(pS),S,tS,"supplementary boot space");
                
                std::cerr << "R  = "  << R  << std::endl;
                std::cerr << "pL = "  << pL << std::endl;
                std::cerr << "dL = "  << dL << std::endl;
                std::cerr << "S  = "  << S  << std::endl;
                std::cerr << "pS = "  << S  << std::endl;
                std::cerr << "dS = "  << dS << std::endl;


            }
            catch(...)
            {
                quit();
                throw;
            }
        }


    }

}


namespace upsylon {

    namespace Aqua
    {
        void Boot:: electroneutrality(const Library &lib)
        {
            Constraint &EN = (*this)(0);
            for(Library::const_iterator it=lib.begin();it!=lib.end();++it)
            {
                const Species &species = **it;
                EN.add(species,species.Z);
            }
        }
    }

}

namespace upsylon {

    namespace Aqua
    {
        void  Boot:: find(addressable<double> &C,
                          Engine              &engine)  
        {
            assert(C.size()>=engine.M);
            // build Cstar
            const size_t   M  = engine.M;
            const size_t   Nc = size;
            vector<double> Cold(M,0);
            vector<double> Cnew(M,0);
            vector<double> Cprj(M,0);
            vector<double> Lambda(Nc,0);
            vector<double> RC(Nc,0);

            fill(Lambda);
            tao::mul(Cold,pL,Lambda);
            tao::divset(Cold,dL);
            std::cerr << "Lambda=" << Lambda << std::endl;
            std::cerr << "Cstar =" << Cold  << std::endl;
            std::cerr << "R     =" << R << std::endl;

            // initial balance
            if( !engine.balance_(Cold, pS, dS) )
            {
                throw exception("no possible initial balance");
            }

            // initial equilibrium
            std::cerr << "Cbal= " << Cold << std::endl;
            tao::set(Cnew,Cold);
            if(!engine.forward(Cnew))
            {
                throw exception("no possible initial forward");
            }

            std::cerr << "Cfwd= " << Cnew << std::endl;

            tao::mul(RC, R, Cnew);
            tao::subp(RC,Lambda);
            tao::mul(Cprj,pL,RC);
            tao::divset(Cprj,dL);
            tao::add(Cnew,Cprj);
            std::cerr << "dC=" << Cprj << std::endl;
            std::cerr << "Cnew= " << Cnew << std::endl;
            if( !engine.balance_(Cnew, pS, dS) )
            {
                throw exception("no possible   balance");
            }
            std::cerr << "C= " << Cnew << std::endl;
        }

    }
}

