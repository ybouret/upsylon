//! \file
#ifndef Y_LANG_SYNTAX_NODE_INTERNAL_INCLUDED
#define Y_LANG_SYNTAX_NODE_INTERNAL_INCLUDED 1

#include "y/lang/syntax/node.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            //! an Internal Node, has a list of children
            class InternalNode : public Node, public Node::List
            {
            public:
                static const id_t UUID = Y_FOURCC(UPFX,'I','N','T');  //!< for I/O

                virtual ~InternalNode() throw();                     //!< destructor
                virtual Node       *  clone() const;                 //!< clone
                virtual const void *  inner() const throw();         //!< this
                void                  vizCore(ios::ostream &) const; //!< graphViz
                virtual const string *data() const throw();          //!< data or NULL

            protected:
                InternalNode(const InternalNode &) throw();             //!< copy
                InternalNode(const Rule &r) throw();                    //!< from rule
                void         vizLink(ios::ostream &) const;             //!< emit links
                size_t       serializeList(ios::ostream&) const;        //!< emit list of children

            private:
                friend class Node;
                Y_DISABLE_ASSIGN(InternalNode);
                virtual void returnTo( Lexer &lexer ) throw();

                virtual const char *className() const throw();
                virtual size_t      serialize(ios::ostream&) const;

            };



        }

    }
    
}
#endif

