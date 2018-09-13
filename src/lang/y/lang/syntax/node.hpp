//!\file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED

#include "y/lang/lexical/unit.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            class Rule;

            class Node : public Object
            {
            public:

                typedef core::list_of<Node> ListType;
                class List : public Object, public ListType
                {
                public:
                    explicit List() throw();
                    virtual ~List() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };

                Node      *next;
                Node      *prev;
                const bool terminal;
                const bool internal;
                virtual ~Node() throw();

                //! create a terminal
                static inline Node *Create(const Rule &r, Lexeme *l)
                {
                    assert(l);
                    auto_ptr<Lexeme> guard(l);
                    Node            *node = new Node(r,l);
                    guard.dismiss();
                    return node;
                }
                //! create an internal
                static inline Node *Create( const Rule &r )
                {
                    return new Node(r);
                }


            private:
                Y_DISABLE_ASSIGN(Node);
                void *impl;
                explicit Node( const Rule &r, Lexeme *lx ) throw() ;
                explicit Node( const Rule &r ) throw();

            public:
                Lexeme     &lexeme;
                Node::List &children;
                const Rule &rule;
            };

        }
    }
}

#endif
