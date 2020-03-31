//! \file

#ifndef Y_JARGON_GRAMMAR_INCLUDED
#define Y_JARGON_GRAMMAR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        typedef suffix_tree<Dogma> Axioms;
        
        class Grammar
        {
        public:
            
            
            virtual ~Grammar() throw();
            
            template <typename LABEL>
            inline Grammar( const LABEL &id ) :
            title( Tags::Make(id) ),
            xcache(title),
            ground(  &(*xcache.dull) ),
            axioms(),
            maxLength(0)
            {
            }
            
            void enroll(Axiom *);
            bool owns(const Axiom &) const throw();
            
            
            
            const Axioms & operator*() const throw();
            const Axiom  & getGround() const throw();
            void           setGround(const Axiom &);
            void           displayAxioms() const;
            
            const Tag title;
            XCache    xcache;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            const Axiom *ground;
            Axioms       axioms;
            
            bool displayAxiom(const Axioms::path &,const Dogma &) const;

        public:
            const size_t maxLength;
        };
        
    }
}

#endif

