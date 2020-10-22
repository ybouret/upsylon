#include "y/aqua/boot.hpp"
#include "y/aqua/engine.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/apk.hpp"
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
                // allocate memory
                //--------------------------------------------------------------
                aliasing::_(R).  make(Nc,M);
                aliasing::_(pL). make(M,Nc); assert(M==dL.size());
                aliasing::_(S).  make(N,M);
                aliasing::_(pS). make(M,M);  assert(M==dS.size());

                //--------------------------------------------------------------
                // fill R from constraints
                //--------------------------------------------------------------
                lib.buildIndices();
                {
                    size_t i=1;
                    for(const Constraint *cc=head;cc;cc=cc->next,++i)
                    {
                        cc->fill( aliasing::_(R[i]) );
                    }
                }
                
                //--------------------------------------------------------------
                // compute 'Lambda => C' matrix, with scaling
                //--------------------------------------------------------------
                {
                    // apk results
                    matrix<apz> aR2(Nc,Nc);
                    apz         dR2 = apk::adjoint_gram(aR2,R);
                    std::cerr << "aR2=" << aR2 << std::endl;
                    std::cerr << "dR2=" << dR2 << std::endl;
                    if(0==dR2)
                    {
                        throw exception("%ssingular set of constraints",fn);
                    }
                    matrix<apz>       Lproj(M,Nc);
                    addressable<apz> &Lscal = Lproj.r_aux1;
                    tao::mmul_ltrn(Lproj,R,aR2);
                    apk::simplify(Lscal,Lproj,dR2);
                    std::cerr << "Lproj = " << Lproj << std::endl;
                    std::cerr << "Lscal = " << Lscal << std::endl;

                    // transfert
                    apk::convert( aliasing::_(pL), Lproj, "Aqua::Boot::convert(<proj>)");
                    apk::convert( aliasing::_(dL), Lscal, "Aqua::Boot::convert(<scal>)");
                    std::cerr << "pL = " << pL << std::endl;
                    std::cerr << "dL = " << dL << std::endl;
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
                Engine::Project(aliasing::_(pS), aliasing::_(dS), S,"supplementary boot space");


                std::cerr << "R  = "  << R  << std::endl;
                std::cerr << "pL = "  << pL << std::endl;
                std::cerr << "dL = "  << dL << std::endl;
                std::cerr << "S  = "  << S  << std::endl;
                std::cerr << "pS = "  << pS << std::endl;
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
