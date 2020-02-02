//! \file
#ifndef Y_LANG_SYNTAX_NODE_TERMINAL_INCLUDED
#define Y_LANG_SYNTAX_NODE_TERMINAL_INCLUDED 1

#include "y/lang/syntax/node.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            //! a Terminal Node, acts as a lexeme smart pointer
            class TerminalNode : public Node
            {
            public:
                static const id_t UUID = Y_FOURCC(UPFX,'T','R','M');    //!< for I/O

                virtual ~TerminalNode() throw();                       //!< destructor
                virtual Node       *  clone() const;                   //!< clone
                virtual const void *  inner() const throw();           //!< lx
                virtual void          vizCore( ios::ostream & ) const; //!< for graphViz
                virtual const string *data() const throw();            //!< NULL

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TerminalNode);
                Lexeme *lx;

                explicit TerminalNode(const Rule &r, Lexeme *l) throw();
                virtual void returnTo( Lexer &lexer ) throw();
                friend class Node;

                virtual const char *className() const throw();
                virtual size_t      serialize(ios::ostream&) const;
            };

         

        }

    }
}


#endif

