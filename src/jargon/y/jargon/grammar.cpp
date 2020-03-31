
#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Grammar:: ~Grammar() throw()
        {
            ground = 0;
        }
 
        void Grammar:: enroll(Axiom *axiom)
        {
            assert( axiom );
            const Dogma   dogma = axiom;
            const string &key   = *(dogma->label);
            if( ! axioms.insert_by(key,dogma) )
            {
                throw exception("[%s] enroll multiple <%s>", **title, *key);
            }
            if( &(*xcache.dull) == ground)
            {
                ground = axiom;
            }
            aliasing::_(maxLength) = max_of(maxLength,key.size());
        }
        
        bool Grammar:: owns(const Axiom &axiom) const throw()
        {
            const Dogma *ppAxiom = axioms.search_by( *(axiom.label) );
            if(!ppAxiom ) return false;
            const Axiom  &local = **ppAxiom;
            return ( &local == &axiom );
        }
        
        const Axioms & Grammar:: operator*() const throw()
        {
            return axioms;
        }
        
        const Axiom  & Grammar:: getGround() const throw()
        {
            assert(ground);
            return *ground;
        }
        
        void Grammar:: setGround(const Axiom &axiom)
        {
            if(!owns(axiom)) throw exception("%s.setGround(foreign <%s>)", **title, **(axiom.label));
            ground = & axiom;
        }

        
        bool Grammar:: displayAxiom(const Axioms::path &, const Dogma &axiom) const
        {
            if( !owns(*axiom) ) return false;
            const string &akey = * axiom->label;
            std::cerr << akey ; assert(akey.size()<=maxLength);
            for(size_t i=maxLength-akey.length();i>0;--i) std::cerr << ' ';
            std::cerr << " [" << fourcc_(axiom->uuid) << "]" << std::endl;
            return true;
        }

        void Grammar:: displayAxioms() const
        {
            functor<bool,TL2(const Axioms::path&,const Dogma&)> cb( this, & Grammar::displayAxiom);
            (void) axioms.for_each(cb);
        }
    }
    
}
