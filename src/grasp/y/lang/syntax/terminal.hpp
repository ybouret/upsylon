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

            //! used to implement flag testing
#define Y_LANG_SYNTAX_TERM_IS(FLAG) inline bool is##FLAG() const throw() \
{ return ( 0 != (attr&FLAG) ); }

            //! standard terminal matching a lexeme
            class Terminal :  public Rule
            {
            public:
                typedef unsigned      attr_t; //!< alias
                static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID
                static const attr_t   Dangling = 0x00; //!< not set
                static const attr_t   Standard = 0x01; //!< a regular expression
                static const attr_t   Univocal = 0x02; //!< a univocal expression, content is dropped
                static const attr_t   Operator = 0x04; //!< an operator terminal
                static const attr_t   Semantic = 0x08; //!< a purely semantic terminal



                explicit Terminal(const string &n);                       //!< setup
                virtual ~Terminal() throw();                              //!< destructor

                Y_LANG_SYNTAX_ACCEPT_PROTO();                             //!< accept a terminal with name==label
                virtual const char *typeName() const throw();             //!< "Terminal"
                virtual bool        isHollow() const throw();             //!< false
                virtual const char *graphVizShape() const throw();        //!< common shape
                virtual void        graphVizEpilog(ios::ostream &) const; //!< nothing to do...
                virtual const char *graphVizStyle() const throw();        //!< change style according to attributes

                const attr_t attr; //!< attributes

                bool isDangling() const throw(); //!< Dangling==attr
                Y_LANG_SYNTAX_TERM_IS(Standard)
                Y_LANG_SYNTAX_TERM_IS(Univocal)
                Y_LANG_SYNTAX_TERM_IS(Operator)
                Y_LANG_SYNTAX_TERM_IS(Semantic)

                Terminal & setStandard(); //!< set standard bit
                Terminal & setUnivocal(); //!< set univocal bit
                Terminal & setOperator(); //!< set operator bit
                Terminal & setSemantic(); //!< set semantic bit

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            };


        }
    }
}


#endif

