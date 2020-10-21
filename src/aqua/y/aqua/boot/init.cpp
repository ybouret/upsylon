#include "y/aqua/boot.hpp"
#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/simplify.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

namespace upsylon
{

    namespace Aqua
    {

        using namespace mkl;

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
