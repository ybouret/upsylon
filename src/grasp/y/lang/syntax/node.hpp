//! \file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED 1

#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace Syntax
        {
            class Rule; //!< forward declaration

            class Node : public Object, public core::inode<Node>
            {
            public:
                typedef core::inode<Node>             Base; //!< alias
                typedef core::list_of_cloneable<Node> List; //!< alias

                const Rule &rule;     //!< creating rule
                const bool  terminal; //!< terminal flag
                const bool  internal; //!< internal flag

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________

                virtual Node *      clone() const         = 0;
                virtual const void *inner() const throw() = 0;
                virtual ~Node() throw();

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                Lexeme       &lexeme() throw();
                const Lexeme &lexeme() const throw();
                List         &children() throw();
                const List   &children() const throw();

                //______________________________________________________________
                //
                // static interface
                //______________________________________________________________
                static Node * Create(const Rule &r, Lexeme *l);   //!< create a new terminal node
                static Node * Create(const Rule &r);              //!< create a new internal node
                static void   Grow( Node * &tree, Node *leaf ) throw();   //!< grew the tree with the leaf
                static void   Unget( Node * &node, Lexer &lexer) throw(); //!< restore lexemes into lexer

            protected:
                explicit Node(const Rule &r,
                              const bool term) throw();
                explicit Node(const Node &other) throw();

            private:
                Y_DISABLE_ASSIGN(Node);
            };

            //! a Terminal Node, acts as a lexeme smart pointer
            class TerminalNode : public Node
            {
            public:
                virtual ~TerminalNode() throw();
                virtual Node       *clone() const;
                virtual const void *inner() const throw();

            private:
                explicit TerminalNode(const Rule &r, Lexeme *l) throw();
                friend class Node;
                Lexeme *lx;

                Y_DISABLE_COPY_AND_ASSIGN(TerminalNode);
            };

            //! an Internal Node, has a list of children
            class InternalNode : public Node, public Node::List
            {
            public:
                //Node::List children;

                virtual ~InternalNode() throw();
                
                virtual Node       *clone() const;
                virtual const void *inner() const throw();

            private:
                explicit InternalNode(const Rule &r) throw();
                explicit InternalNode(const InternalNode &);
                friend class Node;
                Y_DISABLE_ASSIGN(InternalNode);
            };

        }
    }
}

#endif

