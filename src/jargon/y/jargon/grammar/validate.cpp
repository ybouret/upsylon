

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
            if(NULL==root) throw exception("[%s] has no axioms",**title);
            Y_JAXIOM(std::cerr << "[" << title << "] is active..." << std::endl);

            //__________________________________________________________________
            //
            // check
            //__________________________________________________________________
            size_t numTerm = 0;
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                const Axiom &axiom = **it;
                if(Terminal::UUID == axiom.uuid)
                {
                    ++numTerm;
                    Y_JAXIOM(std::cerr << "Terminal<" << axiom.label << ">" << std::endl);
                }
            }
            if(numTerm<=0) throw exception("[%s] has no Terminal",**title);
            Y_JAXIOM(std::cerr << "[" << title << "] has #Terminal=" << numTerm << std::endl);

            
            //__________________________________________________________________
            //
            // load all axioms from the ground
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] loading axioms from root <" << root->label << ">..." << std::endl);
            Y_JAXIOM(std::cerr << "[" << title << "] fetched #axiom=" << axioms.entries() << std::endl);
            Visitor V;
            V.supplyWith(*root);
            Y_JAXIOM(std::cerr << "[" << title << "] visited #axiom=" << V.entries() << std::endl);

            //__________________________________________________________________
            //
            // check that top-level axioms are visited
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] checking standalone axioms..." << std::endl);
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                const Axiom &axiom = **it;
                if( V.search(axiom) ) continue;
                if(allowStandalone)   continue;
                throw exception("[%s] has standalone <%s>", **title, **(axiom.label) );
            }
            
            //__________________________________________________________________
            //
            // check that all visited are registered
            //__________________________________________________________________
            Y_JAXIOM(std::cerr << "[" << title << "] checking foreign axioms..." << std::endl);
            for(Visitor::iterator it=V.begin();it!=V.end();++it)
            {
                const BEaddress &addr  = *it;
                const Axiom     &axiom = *addr.as<Axiom>();
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
            
            V.sort();
            Y_JAXIOM(V.display());
            
            //__________________________________________________________________
            //
            // first pass: revealing all parents
            //__________________________________________________________________
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                Axiom &axiom = **it;
                axiom.revealParents();
            }
            
            //__________________________________________________________________
            //
            //second pass: cleanup
            //__________________________________________________________________
            for(Axioms::iterator it=axioms.begin();it!=axioms.end();++it)
            {
                Axiom &axiom = **it;
                switch(axiom.uuid)
                {
                    case Terminal::UUID:
                    case Operator::UUID:
                        break;
                        
                    default:
                        axiom.parents.release_all();
                        V.remove(axiom);
                        break;
                }
                
            }
            
            Y_JAXIOM(V.display());
            
        }
        
        
       

    }
    
}

