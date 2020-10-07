#include "y/jive/syntax/grammar.hpp"
#include "y/jive/syntax/axiom/all.hpp"

#include "y/exception.hpp"

#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            ios::ostream &Grammar:: graphTag(ios::ostream &fp, const Axiom &axiom) const
            {


                switch(axiom.uuid)
                {
                    case Terminal::UUID:
                        fp << "[label=\"" << axiom.gvName() << "\",shape=box]";
                        break;

                    case Repeat::UUID: {
                        const Repeat &rep = axiom.as<Repeat>();
                        string        aid = rep.axiom.gvName();
                        switch(rep.atLeast)
                        {
                            case  0: aid += "*"; break;
                            case  1: aid += "+"; break;
                            default: aid += vformat(">=%u",unsigned(rep.atLeast));
                                break;
                        }
                        fp << "[label=\"" << aid << "\",shape=oval]";
                    } break;

                    case Option::UUID: {
                        const Option &opt = axiom.as<Option>();
                        string        aid = opt.axiom.gvName() + "?";
                        fp << "[label=\"" << aid << "\",shape=oval]";
                    } break;


                    default:
                        throw exception("%s can't handle Axiom UUID=%s",**name,fourcc_(axiom.uuid));
                }
                return fp;
            }


        }

    }

}

#include "y/ios/tools/graphviz.hpp"
#include "y/ios/ocstream.hpp"
namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {


            void Grammar:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);
                    fp << "digraph G {\n";


                    // declare all axioms
                    for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                    {
                        graphTag(fp.viz(axiom),*axiom) << ";\n";
                    }

                    // make all links
                    for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                    {

                    }
                    fp << "}\n";



                }
                ios::GraphViz::Render(fileName);
            }
        }
    }
}

