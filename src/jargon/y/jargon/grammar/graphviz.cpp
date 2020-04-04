
#include "y/jargon/grammar.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon {
    
    namespace Jargon {

        
        template <typename WILDCARD> static inline
        void gvJoinWC(ios::ostream &fp, const Axiom *parent)
        {
            const Wildcard &w = parent->as<WILDCARD>();
            fp.viz(parent) << "->";
            fp.viz(&w.axiom) << ";\n";
        }
        
        template <typename CMP> static inline
        void gvJoinCM(ios::ostream &fp, const Axiom *axiom)
        {
            const Compound &c = axiom->as<CMP>();
            const bool show = c.size>1;
            unsigned   i    = 1;
            for(const Member *m =c.head;m;m=m->next,++i)
            {
                fp.viz(axiom)  << "->";
                fp.viz(&(m->axiom));
                if(show)
                {
                    fp("[label=\"%u\"]",i);
                }
                fp << ";\n";
            }
        }
        
        static inline void gvJoin( ios::ostream &fp, const Axiom *axiom )
        {
            assert(axiom);
            switch(axiom->uuid)
            {
                case Terminal::UUID:   break;
                case Option::UUID:     gvJoinWC<Option>(fp, axiom);    break;
                case OneOrMore::UUID:  gvJoinWC<OneOrMore>(fp,axiom);  break;
                case ZeroOrMore::UUID: gvJoinWC<ZeroOrMore>(fp,axiom); break;
                   
                case Aggregate::UUID:  gvJoinCM<Aggregate>(fp,axiom);  break;
                case Alternate::UUID:  gvJoinCM<Alternate>(fp,axiom);  break;
                default:
                    break;
            }
            
        }
        
        void Grammar:: graphViz(const string &dotFile, const bool keepFile) const
        {
            {
                ios::ocstream fp(dotFile);
                fp << "digraph " << *title << "{\n";
                
                // all top level rules
                for( Axioms::const_iterator it=axioms.begin(); it != axioms.end(); ++it )
                {
                    const Axiom &axiom = **it;
                    fp.viz(&axiom);
                    fp << '[';
                    fp << "label=\"";
                    for(size_t i=0;i<axiom.label->size();++i)
                    {
                        fp << cchars::printable[ uint8_t( (*axiom.label)[i] ) ];
                    }
                    fp << "\"," << axiom.vizual();
                    fp << ']' << ';' << '\n';
                    
                }
                
                // join top level
                for( Axioms::const_iterator it=axioms.begin(); it != axioms.end(); ++it )
                {
                    gvJoin(fp, & **it);
                }
                
                fp << "}\n";

                
            }
            ios::GraphViz::Render(dotFile,keepFile);
        }
    }
    
}


