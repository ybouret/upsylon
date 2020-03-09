//! \file
#ifndef Y_LANG_SYNTAX_RULE_INCLUDED
#define Y_LANG_SYNTAX_RULE_INCLUDED 1

#include "y/lang/syntax/node.hpp"
#include "y/lang/lexical/lexer.hpp"
#include <typeinfo>

namespace upsylon {

    namespace Lang {

        namespace Syntax {
            //! arguments for accept
#define Y_LANG_SYNTAX_ACCEPT_ARGS Source &source, Lexer &lexer, Node * &tree

            //! accept prototype
#define Y_LANG_SYNTAX_ACCEPT_PROTO() virtual bool         accept( Y_LANG_SYNTAX_ACCEPT_ARGS ) const

            //! accept implementation
#define Y_LANG_SYNTAX_ACCEPT_START(CLASS)    bool CLASS:: accept( Y_LANG_SYNTAX_ACCEPT_ARGS ) const

            //! switchable output on verbose flag
#define Y_LANG_SYNTAX_VERBOSE(OUTPUT) do { if(verbose) { OUTPUT; } } while(false)

            //------------------------------------------------------------------
            //
            //! Rule interface
            /**
             a rule will grow a syntax tree with a source and a lexer
             */
            //------------------------------------------------------------------
            class Rule : public Object, public inode<Rule>
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef core::list_of_cpp<Rule> List; //!< alias

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const uint32_t        uuid;     //!< class identifier
                const string          name;     //!< user's identifier
                const void           *derived;  //!< helper for optimizations
                const std::type_info &info;     //!< derived pointer type info
                bool                  verbose;  //!< for output

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                Y_LANG_SYNTAX_ACCEPT_PROTO()  = 0;                            //!< accept method
                virtual const char *typeName() const throw()     = 0;         //!< get a textual name
                virtual bool        isHollow() const throw()     = 0;         //!< if accepts an empty node
                virtual            ~Rule() throw();                           //!< destructor
                virtual void        graphVizEpilog(ios::ostream &) const = 0; //!< perform necessary linking
                virtual void        checkReady() const = 0;                   //!< check if well designed

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                void graphVizAttach( ios::ostream &fp, const Rule *p, unsigned idx ) const; //!< create a link, numbered if idx>0
                void graphVizProlog( ios::ostream &fp ) const;                              //!< common prolog for all Rules

                //! conversion to derived class with consistency checking
                template <typename RULE_TYPE> inline
                const RULE_TYPE & as() const throw() { checkConsistency( typeid(RULE_TYPE) ); return *static_cast<const RULE_TYPE *>(derived); }

            protected:
                explicit Rule(const uint32_t        u,
                              const string         &n,
                              const std::type_info &tid); //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
                void checkConsistency( const std::type_info &target ) const;
                virtual const char *graphVizShape() const throw() = 0;        //!< shape for graphViz
                virtual const char *graphVizStyle() const throw();            //!< based on isHollow, can be superseeded

            };

        }
    }
}

#endif

