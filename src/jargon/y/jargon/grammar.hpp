//! \file

#ifndef Y_JARGON_GRAMMAR_INCLUDED
#define Y_JARGON_GRAMMAR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        typedef suffix_tree<Dogma> Axioms; //!< alias
        class Terminal;
        
        //----------------------------------------------------------------------
        //
        //! base class for grammar
        /**
         upon validation, the starting axioms are computed, the terminal and the
         not vanishing aggregate.
         */
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
            iAlt(1),
            iAgg(1),
            alpha(),
            beta(),
            maxLength(0)
            {
            }
            
            
            
            //------------------------------------------------------------------
            //
            // basic handling
            //
            //------------------------------------------------------------------

            bool   owns(const Axiom &) const throw();     //!< check owned axiom
            const Axioms & operator*() const throw();     //!< access to database
            const Axiom  & getGround() const throw();     //!< get ground Axiom
            void           setGround(const Axiom &);      //!< set (enrolled) ground Axiom
            void           displayAxioms() const;         //!< display enrolled axioms
            void           enroll(Axiom *);               //!< enroll a valid, new dynamic axiom
            void           display(ios::ostream &) const; //!< display summary
            
            //------------------------------------------------------------------
            //
            //
            // advanced handling
            //
            //
            //------------------------------------------------------------------
           
            //! wrapper to enroll a derived, dynamic axiom
            template <typename AXIOM> inline
            AXIOM & declare( AXIOM *axiom )
            {
                enroll(axiom);
                return *axiom;
            }
            
            //! new terminal
            template <typename LABEL> inline
            Axiom & terminal(const LABEL &id, const Terminal::Feature f = Terminal::Standard)
            {
                return declare( new Terminal(id,f) );
            }
            
            //------------------------------------------------------------------
            // wildcards
            //------------------------------------------------------------------
            
            Axiom & oom(Axiom &); //!< new One Or More Axiom
            Axiom & zom(Axiom &); //!< new Zero Or More Axiom
            Axiom & opt(Axiom &); //!< new Option Axion
            
            //------------------------------------------------------------------
            // aggregates
            //------------------------------------------------------------------
            
            //! new aggregate
            template <typename LABEL> inline
            Aggregate & agg(const LABEL &id)
            {
                return declare( new Aggregate(id) );
            }
            
            Aggregate & agg();                           //!< new design aggregate
            Axiom     & cat(Axiom &a,Axiom &b);          //!< a&&b
            Axiom     & cat(Axiom &a,Axiom &b,Axiom &c); //!< a&&b&&c

            //------------------------------------------------------------------
            // alternates
            //------------------------------------------------------------------
            
            Alternate & alt();//!< new alternate
            Axiom     & choice(Axiom &a, Axiom &b);           //!< make a choice
            Axiom     & choice(Axiom &a, Axiom &b, Axiom &c); //!< make a choice
            
            
            //------------------------------------------------------------------
            //
            // management
            //
            //------------------------------------------------------------------
            
            //! validate it
            void validate(bool allowStandalone=false);
            
            //! main call, try to accept the ground axiom
            XNode *accept(Lexer &, Source &) const;
            
            //! recursive call to AST
            XNode *AST( XNode *xnode ) const throw();
            
            //! for memory
            void   clear( XNode * xnode ) const throw();
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag      title;   //!< grammar title
            mutable XCache xcache;  //!< cache of xnode
            
            //------------------------------------------------------------------
            //
            // graphViz
            //
            //------------------------------------------------------------------
            void graphViz(const string &dotFile, const bool keepFile=false) const; //!< write graphViz
            void graphViz(const char *  dotFile, const bool keepFile=false) const; //!< write graphViz wrapper

            //------------------------------------------------------------------
            //
            //utilities
            //
            //------------------------------------------------------------------
            //! get terminal by label
            const Terminal &getTerminal( const Tag &label ) const;
                        
        protected:
            const   Axiom *ground; //!< ground rule, initially inactive
            Axioms         axioms; //!< top-level axioms
            
           
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            unsigned       iAlt;
            unsigned       iAgg;
            bool displayAxiom(const Axioms::path &,const Dogma &) const;
            XNode *onTerminal(XNode *) const throw();
            XNode *onInternal(XNode *) const throw();
            //! merge child->children into children
            void   amalgamate( XList &children, XNode *child) const throw();
            void   collect(const AlphaList &);
            
        public:
            const AlphaList alpha;      //!< list of first apparent Axiom(s)
            const AlphaList beta;       //!< list of other apparent Axiom(s)
            const size_t    maxLength;  //!< maximum length of labels
            
           
        };
        
    }
}

#endif

