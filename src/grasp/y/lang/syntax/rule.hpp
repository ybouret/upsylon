//! \file
#ifndef Y_LANG_SYNTAX_RULE_INCLUDED
#define Y_LANG_SYNTAX_RULE_INCLUDED 1

#include "y/lang/syntax/node.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! arguments for accept
#define Y_LANG_SYNTAX_ACCEPT_ARGS Source &source, Lexer &lexer, Node * &tree

            //! accept prototype
#define Y_LANG_SYNTAX_ACCEPT_PROTO() virtual bool         accept( Y_LANG_SYNTAX_ACCEPT_ARGS ) const

            //! accept implementation
#define Y_LANG_SYNTAX_ACCEPT_START(CLASS)    bool CLASS:: accept( Y_LANG_SYNTAX_ACCEPT_ARGS ) const

            //! switchable output on verbose flag
#define Y_LANG_SYNTAX_VERBOSE(OUTPUT) do { if(verbose) { std::cerr << OUTPUT; } } while(false)

            //! Rule interface
            class Rule : public Object, public core::inode<Rule>
            {
            public:
                typedef core::list_of_cpp<Rule> List; //!< alias

                const uint32_t uuid;     //!< class identifier
                const string   name;     //!< user's identifier
                const void    *derived;  //!< helper for optimizations
                bool           verbose;  //!< for output

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                Y_LANG_SYNTAX_ACCEPT_PROTO()  = 0;                     //!< accept method
                virtual const char *typeName() const throw() = 0;      //!< get a textual name
                virtual bool        isHollow() const throw() = 0;      //!< if accepts an empty node
                virtual            ~Rule() throw();                    //!< destructor
                virtual const char *graphVizShape() const throw() = 0; //!< shape for graphViz

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                const char *graphVizStyle() const throw(); //!< based on isHollow
                void graphVizName( ios::ostream &fp ) const;
                void graphVizLink( ios::ostream &fp, const Rule *p, unsigned idx ) const;
                void graphVizProlog( ios::ostream &fp ) const;

            protected:
                explicit Rule(const uint32_t i, const string &n); //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }
    }
}

#endif

