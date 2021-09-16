
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include <iomanip>




#include "y/mkl/kernel/apk.hpp"
#include "y/yap/lcm.hpp"
#include "y/core/dnode.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/chemical/sys/flux.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        typedef vector<apq,Allocator> qVector;
        typedef arc_ptr<qVector>      qShared;

#if 0
        template <typename ACTORS>
        static inline
        size_t disable(Flags &available, const ACTORS &arr)
        {
            const size_t n=arr.size();
            for(size_t jj=n;jj>0;--jj)
            {
                const Actor   &a = arr[jj];
                const Species &s = a.sp;
                const size_t   j = s.indx;
                assert(true==available[j]);
                available[j] = false;
            }
            return n;
        }


        static inline
        void simplify( qShared &Q )
        {
            const apz fac = yap::lcm::of_denominators( & (*Q)[1], Q->size() );
            tao::mulset(*Q,fac);
        }
#endif

        typedef ios::vizible Vizible;

        void System::buildOmega()
        {
            const string filename = "omega.dot";
            {
                ios::ocstream fp( filename );

                Vizible::enterDigraph(fp,"omega");

                // write species
                for(size_t j=1;j<=M;++j)
                {
                    const Strain  &s  = *strain[j];
                    const Species &sp = *s;
                    string         shape = "oval";
                    string         style = "bold";

                    switch(s.linkage)
                    {
                        case Single:
                            style += ",dotted";
                            break;

                        case Inside:
                            break;

                        case Intake:
                            shape = "house";
                            style += ",filled";
                            break;

                        case Output:
                            shape = "invhouse";
                            style += ",filled";
                            break;

                        case Siphon:
                            shape  = "trapezium";
                            style += ",filled";
                            break;

                        case Source:
                            shape = "invtrapezium";
                            style += ",filled";
                            break;

                        default:
                            break;
                    }



                    fp.viz(&sp) << "[label=\"" << string_convert::to_printable(sp.name) << "\",shape=" << shape << ",style=\"" << style << "\"];\n";

                }

                // write equilibria
                for(size_t i=1;i<=N;++i)
                {
                    const Primary     &p  = *primary[i];
                    const Equilibrium &eq = *p;
                    const char *shape = "rectangle";

                    fp.viz(&eq) << "[label=\"" << string_convert::to_printable(eq.name) << "\",shape=" << shape << "];\n";

                    for(const CNode *node=eq.used.head();node;node=node->next)
                    {
                        const Compound &comp = ***node;
                        const Actor    &a    = *comp;
                        const Species  &sp   = a.sp;
                        switch(comp.tier)
                        {
                            case Reactant: fp.viz(&sp); Vizible::arrow(fp); fp.viz(&eq);  break;
                            case Product:  fp.viz(&eq); Vizible::arrow(fp); fp.viz(&sp);  break;
                        }
                        if(a.nu>1)
                        {
                            fp << "[label=\"" << vformat("%u",unsigned(a.nu)) << "\"]";
                        }
                        fp << ";\n";
                    }

                }




                Vizible::leaveDigraph(fp);
            }

            ios::GraphViz::Render(filename);

            Flux::Graph G(strain,primary);
            G.graphViz("flux.dot");
            
        }
    }

}
