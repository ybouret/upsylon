#include "y/chemical/system.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

#include <iomanip>

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {


        void System:: buildClusters()
        {
            {
                iMatrix &NuB_ = aliasing::_(NuB);
                NuB_.assign(Nu);
                for(size_t j=M;j>0;--j)
                {
                    if(lineage[j]->state==Flow::Endless)
                    {
                        NuB_.ld_col(j,0);
                    }
                }
            }

            Clusters  &C = aliasing::_(clusters);
            size_t     i = 1;
            Cluster   *c = NULL;

            for(;i<=N;++i)
            {
                if( tao::mod2<unit_t>::of(NuB[i])>0)
                {
                    c = C.start(*primary[i],NuB[i]);
                    break;
                }
            }

            if(c)
            {
                for(++i;i<=N;++i)
                {
                    const iAccessible &nu = NuB[i];
                    if(tao::mod2<unit_t>::of(nu)<=0) continue;

                    if(c->overlaps(nu))
                    {
                        c->grow(*primary[i],nu);
                    }
                    else
                    {
                        c = C.start(*primary[i],nu);
                    }
                }
            }

            Y_CHEMICAL_PRINTLN("    NuB  = " << NuB);
            if(Verbosity)
            {
                clusters.display(std::cerr,2);
            }

            for(const Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
            {
                cluster->compile(lineage);
            }



        }

        void System::buildOmega()
        {


            Y_CHEMICAL_PRINTLN("  <Omega>");

#if 0
            {
                size_t boundedCount = 0;
                for(size_t j=1;j<=M;++j)
                {
                    const Lineage &S       = *lineage[j];
                    const Species &s       = *S;
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
                    std::cerr << "    " << s << " = ";

                    std::cerr << std::endl;
                    for(const Consumer *app=S.consumers.head;app;app=app->next)
                    {
                        const unit_t       nu = app->nu;
                        const Equilibrium &eq = ***app;
                        std::cerr << "     |_consumed by " << eq << " @" << nu << std::endl;

                        // look for each equilibria product
                        for(const ANode *node = eq.prod->head();node;node=node->next)
                        {
                            const Actor   &prod = **node;
                            const Lineage &from = *lineage[prod.sp.indx];
                            if(from.state==Flow::Endless) continue;
                            std::cerr << "       \\_producing " << prod << std::endl;
                        }

                    }

                    for(const Producer *app=S.producers.head;app;app=app->next)
                    {
                        const unit_t       nu = app->nu;
                        const Equilibrium &eq = ***app;
                        std::cerr << "     |_produced by " << eq << " @" << nu << std::endl;

                        // look for each equilibria reactant
                        for(const ANode *node = eq.reac->head();node;node=node->next)
                        {
                            const Actor   &reac = **node;
                            const Lineage &from = *lineage[reac.sp.indx];
                            if(from.state==Flow::Endless) continue;
                            std::cerr << "       \\_consuming " << reac << std::endl;
                        }
                    }


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

                //Y_CHEMICAL_PRINTLN("   Omega0 = " << Omega);
                //GramSchmidt::iOrtho(Om);

            }

            Y_CHEMICAL_PRINTLN("   Nu    = " << Nu);
            Y_CHEMICAL_PRINTLN("   NuB   = " << NuB);
            Y_CHEMICAL_PRINTLN("   Omega = " << Omega);
            Y_CHEMICAL_PRINTLN("  <Omega>");

        }
    }

}
