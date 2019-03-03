//! \file
#ifndef Y_LANG_SYNTAX_TERMINAL_INCLUDED
#define Y_LANG_SYNTAX_TERMINAL_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! different attributes for terminal
            enum Attribute
            {
                Standard, //!< matching a regular expression
                Univocal, //!< matching a univocal expression, content will be removed
                Operator, //!< matching a univocal expression, content will be removed and AST will be changed
                Semantic  //!< syntax only, node will be removed
            };
            
            //! standard terminal matching a lexeme
            class Terminal :  public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID

                
                explicit Terminal(const string &n, const Attribute role = Standard); //!< setup
                virtual ~Terminal() throw();                                         //!< desctructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                             //!< accept a terminal with name==label
                virtual const char *typeName() const throw();             //!< "Terminal"
                virtual bool        isHollow() const throw();             //!< false
                virtual const char *graphVizShape() const throw();        //!< common shape
                virtual void        graphVizEpilog(ios::ostream &) const; //!< nothing to do...

                const Attribute attr;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };


        }
    }
}


#endif

