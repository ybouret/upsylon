//! \file

#ifndef Y_JARGON_GRAMMAR_INCLUDED
#define Y_JARGON_GRAMMAR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        typedef suffix_tree<Dogma> Axioms; //!< alias

        //----------------------------------------------------------------------
        //
        //! dummy Internal for testing
        //
        //----------------------------------------------------------------------
        class Grammar
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~Grammar() throw();
            
            //! setup
            template <typename LABEL>
            inline Grammar( const LABEL &id ) :
            title( Tags::Make(id) ),
            xcache(title),
            ground(  &(*xcache.dull) ),
            axioms(),
            maxLength(0)
            {
            }
            
            
            
            //------------------------------------------------------------------
            //
            // basic handling
            //
            //------------------------------------------------------------------

            bool   owns(const Axiom &) const throw(); //!< check owned axiom
            const Axioms & operator*() const throw(); //!< access to database
            const Axiom  & getGround() const throw(); //!< get ground Axiom
            void           setGround(const Axiom &);  //!< set (enrolled) ground Axiom
            void           displayAxioms() const;     //!< display enrolled axioms
            void           enroll(Axiom *);           //!< enroll a valid, new dynamic axiom

            //------------------------------------------------------------------
            //
            // advanced handling
            //
            //------------------------------------------------------------------
           
            //! wrapper to enroll a derived, dynamic axiom
            template <typename AXIOM> inline
            AXIOM & declare( AXIOM *axiom )
            {
                enroll(axiom);
                return *axiom;
            }
            
            Axiom & oom(Axiom &); //!< new One Or More Axiom
            Axiom & zom(Axiom &); //!< new Zero Or More Axiom
            Axiom & opt(Axiom &); //!< new Option Axion
            
            //! new terminal
            template <typename LABEL> inline
            Axiom & terminal(const LABEL &id)
            {
                return declare( new Terminal(id) );
            }
            
            //! new aggregate
            template <typename LABEL> inline
            Aggregate & agg(const LABEL &id)
            {
                return declare( new Aggregate(id) );
            }
            
            //! new alternate
            template <typename LABEL> inline
            Alternate & alt(const LABEL &id)
            {
                return declare( new Alternate(id) );
            }
            
            //------------------------------------------------------------------
            //
            //! main call, try to accept the ground axiom
            //
            //------------------------------------------------------------------
            XNode *accept(Lexer &, Source &) const;
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag      title;   //!< grammar title
            mutable XCache xcache;  //!< cache of xnode
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            const   Axiom *ground;
            Axioms       axioms;
            
            bool displayAxiom(const Axioms::path &,const Dogma &) const;
            const char *nameOf(const Axiom *accepted) const throw();
            
        public:
            const size_t maxLength; //!< maximum length of labels
        };
        
    }
}

#endif

