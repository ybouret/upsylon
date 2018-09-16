//!\file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED

#include "y/lang/lexical/translator.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            class Rule; //!< forward

            //! a node of the AST
            class Node : public Object
            {
            public:

                typedef core::list_of<Node> ListType; //!< base class

                //! for children nodes
                class List : public Object, public ListType
                {
                public:
                    explicit List() throw(); //!< initialize
                    virtual ~List() throw(); //!< destructor, NO cleanup!

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };

                Node      *next;     //!< for list
                Node      *prev;     //!< for list
                const bool terminal; //!< if is terminal
                const bool internal; //!< !terminal

                virtual ~Node() throw(); //!< destructor

                //! create a terminal node
                static inline Node *Create(const Rule &r, Lexeme *l)
                {
                    assert(l);
                    auto_ptr<Lexeme> guard(l);
                    Node            *node = new Node(r,l);
                    guard.dismiss();
                    return node;
                }

                //! create an internal node
                static inline Node *Create( const Rule &r )
                {
                    return new Node(r);
                }

                //! helper to grow child
                inline void add( Node *child ) throw()
                {
                    assert(internal);
                    children.push_back(child);
                }

                //! write content as GraphViz
                void viz( ios::ostream &fp ) const;

                //! write a directed graph
                void        GraphViz( const string &fn, bool keepFile=false) const;

                //! write a directed graph
                inline void GraphViz( const char   *fn, bool keepFile=false) const { const string _ = fn; GraphViz(_,keepFile); }

                //! yield terminal lexeme and delete node memory
                static Lexeme *YieldLexeme(Node *node) throw();

                //! return into lexer
                static void BackTo( Lexical::Translator &lexer, Node *node ) throw();

                //! return AST operations
                static Node *AST( Node *node ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
                void *impl;
                explicit Node( const Rule &r, Lexeme *lx ) throw() ;
                explicit Node( const Rule &r ) throw();

            public:
                Lexeme     &lexeme;    //!< hollow lexeme
                Node::List &children;  //!< hollow children
                const Rule &rule;      //!< generating rule
            };

        }
    }
}

#endif
