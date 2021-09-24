#include "y/chemical/system.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        void System::buildOmega()
        {



            Y_CHEMICAL_PRINTLN("  <Omega>");

#if 0
            {
                size_t boundedCount = 0;
                for(size_t j=1;j<=M;++j)
                {
                    const Lineage &S = *lineage[j];
                    const Species &s = *S;
                    bool           process = false;
                    switch(S.linkage)
                    {
                        case Intake:
                        case Output:
                        case Inside:
                            assert(Flow::Bounded==S.state);
                            process=true;
                            break;

                        case Single:
                            assert(Flow::Bounded==S.state);
                            break;

                        case Siphon:
                        case Source:
                            assert(Flow::Endless==S.state);
                            break;
                    }
                    if(!process) continue;
                    ++boundedCount;
                    std::cerr << "    d_t " << s << " = ";

                    std::cerr << std::endl;

                }
                assert(MB==boundedCount);
            }
#endif

            if(Nc>0)
            {
                iMatrix &Om      = aliasing::_(Omega);
                size_t   ortho   = Nc;

                if(charged)
                {
                    --ortho;
                    // Z is in ortho(Nu)
                    for(size_t i=N;i>0;--i)
                    {
                        if(0!=tao::dot<unit_t>::of(Nu[i],Z))
                        {
                            const Equilibrium &eq = **primary[i];
                            throw exception("UNEXPECTED <%s> charge conservation failure!!", *eq.name);
                        }
                    }

                    tao::set(Om[1],Z);

                    if(ortho>0)
                    {
                        // register as extra condition
                        const size_t  iz = N+1;
                        iMatrix U(iz,M);
                        for(size_t i=N;i>0;--i)
                        {
                            tao::set(U[i],Nu[i]);
                        }
                        tao::set(U[iz],Z);
                        iMatrix V(M-iz,M);
                        apk::complete_ortho(U,V);
                        //Y_CHEMICAL_PRINTLN("   U     = " << U);
                        //Y_CHEMICAL_PRINTLN("   V     = " << V);
                        for(size_t i=2,k=1;i<=Nc;++i,++k)
                        {
                            tao::set(Om[i],V[k]);
                        }
                    }

                }
                else
                {
                    // full search
                    apk::complete_ortho(Nu,Om);
                }

                // Y_CHEMICAL_PRINTLN("   Omega0 = " << Omega);
                // GramSchmidt::iOrtho(Om);

            }



            Y_CHEMICAL_PRINTLN("   Omega = " << Omega);
            Y_CHEMICAL_PRINTLN("  <Omega>");

        }
    }

}
