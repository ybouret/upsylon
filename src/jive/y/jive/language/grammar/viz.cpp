#include "y/jive/language/grammar.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            
            namespace
            {
                
                
                static inline void gvAxiom(ios::ostream &fp, const Axiom *axiom)
                {
                    fp.viz(axiom) << '[';
                    
                    fp << "label=\"" << axiom->gvName() << "\"";
                    
                    Vizible::endl(fp << ']');
                }
                
                static inline void gvLink1(ios::ostream  &fp,
                                           const Axiom   *parent,
                                           const Axiom   *child,
                                           const unsigned indx=1,
                                           const unsigned size=1)
                {
                    assert(parent);
                    assert(child);
                    
                    fp.viz(parent); Vizible::arrow(fp); fp.viz(child);
                    
                    if(size>1)
                    {
                        fp << vformat("[label=\"%u\"]",indx);
                    }
                    
                    Vizible::endl(fp);
                }
                
                static inline void gvLinkN(ios::ostream  &fp,
                                           const Compound &cm)
                {
                    const unsigned size = unsigned(cm.size);
                    unsigned       indx = 1;
                    for(const Axiom::Reference *ref=cm.head;ref;ref=ref->next,++indx)
                    {
                        gvLink1(fp, &cm, & **ref, indx, size);
                    }
                    
                }
                
                
#define Y_GV_LINK_WILDCARD(CLASS) case CLASS::UUID: gvLink1(fp,axiom,& axiom->as<CLASS>().axiom); break
#define Y_GV_LINK_COMPOUND(CLASS) case CLASS::UUID: gvLinkN(fp,axiom->as<CLASS>()); break
                
                
                static inline void gvLinks(ios::ostream &fp, const Axiom *axiom)
                {
                    switch(axiom->uuid)
                    {
                            Y_GV_LINK_WILDCARD(Repeat);
                            Y_GV_LINK_WILDCARD(Option);
                            Y_GV_LINK_COMPOUND(Aggregate);
                            Y_GV_LINK_COMPOUND(Alternate);
                        case Terminal::UUID: break;
                        default: throw exception("[Jive::Gramma::gvLinks(unhandled type %s)]", fourcc_(axiom->uuid));
                    }
                    
                }
                
            }
            
            void   Grammar:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);
                    Vizible::enterDigraph(fp, "G");
                    
                    //----------------------------------------------------------
                    //
                    // first pass: write axiom
                    //
                    //----------------------------------------------------------
                    for(const Axiom *axiom = axioms.head;axiom;axiom=axiom->next)
                    {
                        gvAxiom(fp,axiom);
                    }
                    
                    //----------------------------------------------------------
                    //
                    // second pass: write links
                    //
                    //----------------------------------------------------------
                    for(const Axiom *axiom = axioms.head;axiom;axiom=axiom->next)
                    {
                        gvLinks(fp,axiom);
                    }
                    
                    Vizible::leaveDigraph(fp);
                    
                }
                
                ios::GraphViz::Render(fileName,true);
                
            }
            
            
            
        }
        
    }
    
}

