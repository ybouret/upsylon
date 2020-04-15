//! \file

#ifndef Y_JARGON_GRAMMAR_INCLUDED
#define Y_JARGON_GRAMMAR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        typedef suffix_tree<Dogma> Axioms; //!< alias
        class Terminal;
        
        //----------------------------------------------------------------------
        //
        //! base class for grammar
        /**
         a grammar is a set of axioms
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
            ground(NULL),
            axioms(),
            iAlt(1),
            iAgg(1),
            maxLength(0),
            buildAST(true)
            {
            }
            
            
            
            //------------------------------------------------------------------
            //
            // basic handling
            //
            //------------------------------------------------------------------
            bool   owns(const Axiom &) const throw();     //!< check owned axiom
            const Axioms & operator*() const throw();     //!< access to database
            bool           hasGround() const throw();     //!< check if has ground rule
            const Axiom  & getGround() const throw();     //!< get ground Axiom
            void           setGround(const Axiom &);      //!< set (enrolled) ground Axiom
            void           displayAxioms() const;         //!< display enrolled axioms
            void           enroll(Axiom *);               //!< enroll a valid, new dynamic axiom
            
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
            
            //! new Terminal, with feature
            template <typename LABEL> inline
            Axiom & _terminal(const LABEL             &id,
                              const Terminal::Feature  ft)
            {
                return declare( new Terminal(id,ft) );
            }
            
            //! new Operator, with attribute
            template <typename LABEL> inline
            Axiom & _operator(const LABEL               &id,
                              const Operator::Attribute &attr)
            {
                return declare( new Operator(id,attr) );
            }
            
            //------------------------------------------------------------------
            // wildcards
            //------------------------------------------------------------------
            
            Axiom & oom(Axiom &); //!< new One Or More Axiom
            Axiom & zom(Axiom &); //!< new Zero Or More Axiom
            Axiom & opt(Axiom &); //!< new Option Axiom
            
            //------------------------------------------------------------------
            // aggregates
            //------------------------------------------------------------------
            
            //! new aggregate
            template <typename LABEL> inline
            Aggregate & agg(const LABEL &id)
            {
                return declare( new Aggregate(id) );
            }
            
            //! new acting aggregate
            template <typename LABEL> inline
            Aggregate & act(const LABEL &id)
            {
                return declare( new Aggregate(id,Aggregate::AsActing) );
            }
            
            Aggregate & agg();                           //!< new vanishing aggregate
            Axiom     & cat(Axiom &a,Axiom &b);          //!< a&&b
            Axiom     & cat(Axiom &a,Axiom &b,Axiom &c); //!< a&&b&&c

            //------------------------------------------------------------------
            // alternates
            //------------------------------------------------------------------
            
            //! creating a named alternate
            template <typename LABEL> inline
            Alternate & alt(const LABEL &id)
            {
                return declare( new Alternate(id) );
            }
            
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
            
           
            
            //! compact/rewrite
            XNode *AST(XNode *xnode) const throw();
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag      title;   //!< grammar title
            
            //------------------------------------------------------------------
            //
            // graphViz
            //
            //------------------------------------------------------------------
            void graphViz(const string &dotFile, const bool keepFile=false) const; //!< write graphViz
            void graphViz(const char *  dotFile, const bool keepFile=false) const; //!< write graphViz wrapper

            //------------------------------------------------------------------
            //
            // utilities
            //
            //------------------------------------------------------------------
            
            //! get terminal by label, NULL if not registered
            const Terminal *toTerminal(const Tag &) const throw();
            
            //! low level reload tree
            XNode *loadTree(Context      &where,
                            ios::istream &input) const;
            
            
        protected:
            const   Axiom *ground; //!< ground rule, initially inactive
            Axioms         axioms; //!< top-level axioms
            
           
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            unsigned       iAlt;
            unsigned       iAgg;
            
            bool displayAxiom(const Axioms::path &,const Dogma &) const;
            
            XNode *compact(XNode *xnode) const throw();
            XNode *rewrite(XNode *xnode) const throw();
            
            XNode *onTerminal(XNode *) const throw(); //!< cleanup terminal
            XNode *onOperator(XNode *) const throw(); //!< cleanup operator
            XNode *onInternal(XNode *) const throw(); //!< cleanup internal
            
            void   reduceAST(XList &)  const throw();
            void   fusionAST(XList &)  const throw();
            
            const Axiom & loadAxiom(ios::istream &fp, int &r, const char *which) const;
            
        public:
            const size_t    maxLength;  //!< maximum length of labels
            bool            buildAST;   //!< default is true
        };
        
    }
}

#endif

