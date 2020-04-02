

#include "y/jargon/grammar.hpp"
#include "y/jargon/axiom/visitor.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        
        void Grammar:: compile(const bool allowStandalone)
        {
            Y_JAXIOM(std::cerr << "[" << title << "] syntax compilation" << std::endl);
           
            //__________________________________________________________________
            //
            // is it active ?
            //__________________________________________________________________
            if( ground == & *xcache.dull )
            {
                throw exception("[%s] has no axioms",**title);
            }
            
            //__________________________________________________________________
            //
            // load all axioms from the ground
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "|_loading axioms from ground <" << ground->label << ">" << std::endl);
            Y_JAXIOM(std::cerr << "|_grammar #axiom=" << axioms.entries() << std::endl);
            Visitor V;
            V.supplyWith(*ground);
            Y_JAXIOM(std::cerr << "|_visited #axiom=" << V.entries() << std::endl);

            //__________________________________________________________________
            //
            // check that top-level axioms are visited
            //__________________________________________________________________
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                const Axiom &axiom = **it;
                if( V.wasVisited(axiom) ) continue;
                Y_JAXIOM(std::cerr << " |_standalone <" << axiom.label << ">" << std::endl);
                if(allowStandalone) continue;
                throw exception("[%s] has standalone <%s>", **title, **(axiom.label) );
            }
            
            //__________________________________________________________________
            //
            // check that all visited are registered
            //__________________________________________________________________
            for(Visitor::iterator it=V.begin();it!=V.end();++it)
            {
                const AxiomAddress &addr = *it;
                const Axiom        &axiom = *static_cast<const Axiom*>( addr[0] );
                Y_JAXIOM(std::cerr << " |_checking <" << axiom.label << ">" << std::endl);
                if( !owns(axiom) )
                    throw exception("[%s] has foreign <%s>", **title, **axiom.label);
            }
            
            //__________________________________________________________________
            //
            // did I forget something ?
            //__________________________________________________________________
            if( !allowStandalone && (V.entries() != axioms.entries()) )
            {
                throw exception("[%s] unexpected visited entries",**title);
            }
            
            
        }
    }
    
}

