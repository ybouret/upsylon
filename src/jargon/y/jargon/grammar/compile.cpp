

#include "y/jargon/grammar.hpp"
#include "y/jargon/axiom/visitor.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {

        
        void Grammar:: validate(const bool allowStandalone)
        {
            Y_JAXIOM(std::cerr << "[" << title << "] compilation..." << std::endl);
           
            //__________________________________________________________________
            //
            // is it active ?
            //__________________________________________________________________
            if( ground == & *xcache.dull ) throw exception("[%s] has no axioms",**title);
            Y_JAXIOM(std::cerr << "[" << title << "] is active..." << std::endl);

            //__________________________________________________________________
            //
            // initialize reverse graph
            //__________________________________________________________________
        
            size_t numTerm = 0;
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                const Axiom &axiom = **it;
                if(axiom.isTerminal())
                {
                    ++numTerm;
                }
            }
            if(numTerm<=0) throw exception("[%s] has no Terminal",**title);
            Y_JAXIOM(std::cerr << "[" << title << "] has #Terminal=" << numTerm << std::endl);

            
            //__________________________________________________________________
            //
            // load all axioms from the ground
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] loading axioms from ground <" << ground->label << ">..." << std::endl);
            Y_JAXIOM(std::cerr << "[" << title << "] fetched #axiom=" << axioms.entries() << std::endl);
            Visitor V;
            V.supplyWith(*ground);
            Y_JAXIOM(std::cerr << "[" << title << "] visited #axiom=" << V.entries() << std::endl);

            //__________________________________________________________________
            //
            // check that top-level axioms are visited
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] checking standalone axioms..." << std::endl);
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                const Axiom &axiom = **it;
                if( V.wasVisited(axiom) ) continue;
                if(allowStandalone) continue;
                throw exception("[%s] has standalone <%s>", **title, **(axiom.label) );
            }
            
            //__________________________________________________________________
            //
            // check that all visited are registered
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] checking foreign axioms..." << std::endl);
            for(Visitor::iterator it=V.begin();it!=V.end();++it)
            {
                const AxiomAddress &addr = *it;
                const Axiom        &axiom = *static_cast<const Axiom*>( addr[0] );
                if( !owns(axiom) ) throw exception("[%s] has foreign <%s>", **title, **axiom.label);
            }
            
            //__________________________________________________________________
            //
            // did I forget something ?
            //__________________________________________________________________
            if( !allowStandalone && (V.entries() != axioms.entries()) )
            {
                throw exception("[%s] unexpected visited entries",**title);
            }
            
            //__________________________________________________________________
            //
            // build the first list
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] building alpha list..." << std::endl);
            {
                AlphaList &apparent = aliasing::_(alpha);
                apparent.release();
                ground->joinFirstApparentTo(apparent);
            }
            Y_JAXIOM(std::cerr << "[" << title << "] #alpha=" << alpha.size << std::endl);
            for(const AlphaNode *m = alpha.head;m;m=m->next)
            {
                const Axiom &axiom = m->axiom;
                Y_JAXIOM(std::cerr << "|_<" <<  axiom.label << ">" << std::endl);
                
                if(!axiom.joinFirstTerminalTo(aliasing::_(m->terms)))
                {
                    throw exception("[%s] <%s> has no starting terminal!", **title, **(m->axiom.label) );
                }
                
                for(const TermNode *t=m->terms.head;t;t=t->next)
                {
                    Y_JAXIOM(std::cerr << " |_<" <<  t->axiom.label << ">" << std::endl);
                }
            }
        }
        
        
       

    }
    
}

