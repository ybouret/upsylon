//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"
#include "y/associative/ledger.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        //----------------------------------------------------------------------
        //
        //
        // Helpers
        //
        //
        //----------------------------------------------------------------------
        
        //! accept methods
#define Y_JARGON_AXIOM_ARGS_ \
XNode * &xtree,              \
Lexer   &lexer,              \
Source  &source
        
        //! accept method arguments
#define Y_JARGON_AXIOM_ARGS Y_JARGON_AXIOM_ARGS_
        
        //! accept call wrapper
#define Y_JARGON_AXIOM_ACCEPT(TREE) accept(TREE,lexer,source)
        
        //! for verbosity
#define Y_JAXIOM(CODE) do { if(Axiom::Verbose) { CODE; } } while(false)

        //----------------------------------------------------------------------
        //
        //! base class for a syntax axiom
        //
        //----------------------------------------------------------------------
        class Axiom : public  CountedObject
        {
        public:
            //----------------------------------------------------------------------
            //
            // types and definitions
            //
            //----------------------------------------------------------------------
            static bool               Verbose; //!< global verbosity flag
            typedef ledger_of<Axiom>  DB_Type; //!< base class for database
            
            //! lightweigth database
            class DB : public DB_Type
            {
            public:
                explicit DB();                            //!< setup
                virtual ~DB() throw();                    //!< cleanup
                void display(const char *pfx=0) const;    //!< display with parents
                void sort();                              //!< sort by label
                void cat(exception &excp) const throw();  //!< append named to exception
            private:
                Y_DISABLE_COPY_AND_ASSIGN(DB);
            };
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Axiom() throw(); //!< cleanup
            
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual const char *   vizual() const throw()            = 0; //!< graphViz shape+style
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const = 0; //!< main call
            virtual ios::ostream & display(ios::ostream &)     const = 0; //!< display
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag      label; //!< shared label
            const uint32_t uuid;  //!< UUID for I/O
            
            //------------------------------------------------------------------
            //
            // helpers
            //
            //------------------------------------------------------------------
            
            //! recall derived class
            template <typename AXIOM> inline
            AXIOM & as( ) throw()
            {
                assert(AXIOM::UUID==uuid);
                assert(self);
                return *static_cast<AXIOM*>(self);
            }
            
            //! recall const derived class
            template <typename AXIOM> inline
            const AXIOM & as( ) const throw()
            {
                assert(AXIOM::UUID==uuid);
                assert(self);
                return *static_cast<const AXIOM*>(self);
            }
            
            bool isTerminal() const throw(); //!< terminal
            bool isOperator() const throw(); //!< operator
            bool isCompound() const throw(); //!< alt|agg
            bool isApparent() const throw(); //!< not vanishing aggregate
            bool isWildcard() const throw(); //!< option/repeat
            
            
        protected:
            void           *self; //!< pointer to derived class
            
            //! setup for derived classes
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u),
            self(0),
            parents()
            {
            }
            
         
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            void collect( DB &all ) const;

        public:
            DB   parents;  //!< database of possible parents
            void reveal(); //!< reveal the permanent parents
        
        };
        
       

       
        
    }
}

#endif

