#include "y/chemical/sys/flux.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon
{
    namespace Chemical
    {
        void Flux::Vertex::vizCore(ios::ostream &fp) const
        {
            string label;
            string shape;
            string style;
            switch(genus)
            {
                case IsPrimary: {
                    const Primary     &pp = *primary;
                    const Equilibrium &eq = *pp;
                    shape = "rectangle";
                    style = "bold";
                    if(eq.state==Flow::Endless)
                    {
                        style += ",dashed";
                    }

                    label = string_convert::to_printable(eq.name);

                } break;

                case IsStrain: {
                    const Strain  &S = *strain;
                    const Species &s = *S;
                    shape = "oval";
                    style = "bold";
                    
                    switch(S.linkage)
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
                            style += ",dashed,filled";
                            break;

                        case Source:
                            shape = "invtrapezium";
                            style += ",dashed,filled";
                            break;

                        default:
                            break;
                    }

                    label = string_convert::to_printable(s.name);

                } break;
            }

            fp << "[label=\"" << label << "\",shape=\"" << shape << "\",style=\"" << style << "\"]";
            endl(fp);
        }

        void Flux:: Graph:: graphViz(const string &fileName) const
        {
            {
                ios::ocstream fp(fileName);

                Vizible::enterDigraph(fp,"G");

                Save(fp,svtx);
                Save(fp,pvtx);

                Vizible::leaveDigraph(fp);
            }
            ios::GraphViz::Render(fileName,true);
        }

    }

}
