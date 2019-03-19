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

            //! attributes for terminal
            enum Attribute
            {
                Standard, //!< standard terminal
                Operator, //!< terminal is an operator
                Semantic  //!< terminal is a semantic marker
            };

            //! standard terminal matching a lexeme
            class Terminal :  public Rule
            {
            public:
                static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID

                explicit Terminal(const string &n);                       //!< setup
                virtual ~Terminal() throw();                              //!< destructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                             //!< accept a terminal with name==label
                virtual const char *typeName() const throw();             //!< "Terminal"
                virtual bool        isHollow() const throw();             //!< false
                virtual const char *graphVizShape() const throw();        //!< common shape
                virtual void        graphVizEpilog(ios::ostream &) const; //!< nothing to do...
                virtual const char *graphVizStyle() const throw();        //!< change style according to attributes
                virtual void        checkReady() const;                   //!< ok...

                const bool      ordinary; //!< initialize : true
                const bool      univocal; //!< initialize : false
                const Attribute attr;     //!< Standard | Operator | Semantic

                Terminal & op(); //!< set operator attribute
                Terminal & sm(); //!< set semantic attribute
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };


        }
    }
}


#endif

