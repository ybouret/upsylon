#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

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
                    case Terminal::UUID: {
                        const char *style = "bold,filled";
                        switch(axiom.as<Terminal>().type)
                        {
                            case Terminal::Standard: break;
                            case Terminal::Univocal: style="bold,filled,rounded"; break;
                            case Terminal::Division: style="bold,filled,dashed";  break;
                        }
                        fp << "[label=\"" << axiom.gvName() << "\",shape=box,style=\"" << style << "\"]";
                    } break;

                    case EndLine::UUID: {
                        const char    *style = "bold,filled";
                        fp << "[label=\"" << axiom.gvName() << "\",shape=box,style=\"" << style << "\"]";
                    } break;



                    case Repeat::UUID: {
                        fp << "[label=\"" << axiom.gvName() << "\",shape=oval]";
                    } break;

                    case Option::UUID: {
                        fp << "[label=\"" << axiom.gvName() << "\",shape=diamond]";
                    } break;

                    case Aggregate::UUID: {
                        const Aggregate &cmp = axiom.as<Aggregate>();
                        const string     aid = cmp.gvName();
                        fp << "[label=\"" << aid << "\",shape=house,style=\"";
                        switch(cmp.type)
                        {
                            case Aggregate::Standard: fp << "bold,filled";         break;
                            case Aggregate::Variadic: fp << "bold,filled,rounded"; break;
                            case Aggregate::Grouping: fp << "solid";               break;
                        }
                        fp << "\"]";
                    } break;

                    case Alternate::UUID: {
                        const Compound &cmp = axiom.as<Alternate>();
                        const string    aid = cmp.gvName();
                        fp << "[label=\"" << aid << "\",shape=folder]";
                    } break;

                    default:
                        throw exception("%s can't graphTag Axiom UUID=%s",**name,fourcc_(axiom.uuid));
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


            static inline
            void vizLink(ios::ostream &fp,
                         const Axiom  *parent,
                         const Axiom  *child,
                         const size_t  value=1,
                         const size_t  total=1)
            {
                fp.viz(parent);
                fp << "->";
                fp.viz(child);
                if(total>1)
                {
                    fp("[label=\"%u\"]", unsigned(value));
                }
                fp << ";\n";
            }

            static inline
            void vizLinks(ios::ostream   &fp,
                          const Compound &cmp)
            {
                const size_t  total  = cmp.size;
                const Axiom  *parent = &cmp;
                size_t        value  = 1;
                for(const Axiom::Reference *ref=cmp.head;ref;ref=ref->next,++value)
                {
                    vizLink(fp,parent, & **ref, value, total);
                }
            }



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
                        switch(axiom->uuid)
                        {
                            case Repeat::UUID:    vizLink(fp,axiom,&(axiom->as<Repeat>().axiom)); break;
                            case Option::UUID:    vizLink(fp,axiom,&(axiom->as<Option>().axiom)); break;
                            case Aggregate::UUID: vizLinks(fp,axiom->as<Aggregate>());            break;
                            case Alternate::UUID: vizLinks(fp,axiom->as<Alternate>());            break;
                            default:
                                break;
                        }
                    }
                    fp << "}\n";



                }
                ios::GraphViz::Render(fileName);
            }
        }
    }
}

