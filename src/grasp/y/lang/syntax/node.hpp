//! \file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED 1

#include "y/lang/lexical/translator.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace Syntax
        {
            class Rule;      //!< forward declaration
            class Grammar;   //!< forward declaration

            //! store syntax trees
            class Node : public Object, public core::inode<Node>
            {
            public:
                typedef core::inode<Node>             Base; //!< alias
                typedef core::list_of_cloneable<Node> List; //!< alias
                typedef arc_ptr<const string>         Data; //!< alias

                const Rule &rule;     //!< creating rule
                const bool  terminal; //!< terminal flag
                const bool  internal; //!< internal flag

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual Node *        clone() const               = 0; //!< clone the node
                virtual const void   *inner() const throw()       = 0; //!< internal address
                virtual void          viz( ios::ostream & ) const = 0; //!< output graphViz code
                virtual void          returnTo( Lexer & ) throw() = 0; //!< restore before delete
                virtual const string *data() const throw()        = 0; //!< return data if any
                virtual ~Node() throw();                               //!< destructor
                
                
                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                Lexeme       &lexeme() throw();                       //!< from inner
                const Lexeme &lexeme() const throw();                 //!< from inner
                List         &children() throw();                     //!< from inner
                const List   &children() const throw();               //!< from inner
                void          graphVizName( ios::ostream &fp) const;  //!< helper: fp.viz(this)
                void          graphViz( const string &dotfile) const; //!< save to graphviz and try to render
                void          graphViz( const char   *dotfile) const; //!< save to graphviz and try to render
                void          save( ios::ostream &fp ) const;         //!< save to stream
                void          save( const string &binfile) const;     //!< save to file
                void          save( const char   *binfile) const;     //!< save to file
                string        toBinary() const;                       //!< to a binary string
                string        toBase64() const;                       //!< to a human readable string

                //______________________________________________________________
                //
                // static interface
                //______________________________________________________________
                static Node * Create(const Rule &r, Lexeme *l);           //!< create a new terminal node
                static Node * Create(const Rule &r);                      //!< create a new internal node
                static Node * Create(const Rule &r, const string &s);     //!< create a new internal node with data
                static void   Grow( Node * &tree, Node *leaf )  throw();  //!< grew the tree with the leaf
                static void   Unget(Node * &node, Lexer &lexer) throw();  //!< restore lexemes into lexer

                //! reload node from a binary source
                /**
                 All the CharInfo are CharInfo(source.module.origin,0,0),
                 since we assume the source to be a binary source
                 */
                static Node  *Load( Source &source, Grammar &G);

                //! transform a raw node into the direct AST
                static Node * AST( Node *node ) throw();

                //! rewrite an AST according to operators
                static Node * Rewrite( Node *node, const Grammar &G );


            protected:
                explicit Node(const Rule &r, const bool term) throw(); //!< setup
                Node(const Node &other) throw();                       //!< copy

            private:
                Y_DISABLE_ASSIGN(Node);
                virtual void        emit( ios::ostream & ) const = 0;

            };

            //! a Terminal Node, acts as a lexeme smart pointer
            class TerminalNode : public Node
            {
            public:
                static const uint8_t MAGIC_BYTE = 0x00;            //!< for I/O

                virtual ~TerminalNode() throw();                   //!< destructor
                virtual Node       *  clone() const;               //!< clone
                virtual const void *  inner() const throw();       //!< lx
                virtual void          viz( ios::ostream & ) const; //!< graphViz
                virtual const string *data() const throw();        //!< NULL

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TerminalNode);
                Lexeme *lx;

                explicit TerminalNode(const Rule &r, Lexeme *l) throw();
                virtual void emit( ios::ostream & ) const;
                virtual void returnTo( Lexer &lexer ) throw();
                friend class Node;
            };

            //! an Internal Node, has a list of children
            class InternalNode : public Node, public Node::List
            {
            public:
                static const uint8_t MAGIC_BYTE = 0x01;            //!< for I/O

                virtual ~InternalNode() throw();                   //!< destructor
                virtual Node       *  clone() const;               //!< clone
                virtual const void *  inner() const throw();       //!< this
                void                  viz( ios::ostream & ) const; //!< graphViz
                virtual const string *data() const throw();        //!< data or NULL

            protected:
                InternalNode(const InternalNode &) throw();   //!< copy
                InternalNode(const Rule &r) throw();          //!< from rule
                virtual void emit( ios::ostream & ) const;    //!< emit binary
                void         emitList( ios::ostream &) const; //!< emit list of children
                void         vizLink( ios::ostream & ) const; //!< emit links
            private:
                friend class Node;
                Y_DISABLE_ASSIGN(InternalNode);
                virtual void returnTo( Lexer &lexer ) throw();
                
            };

            //! an Extended Node is an internal node with data
            class ExtendedNode : public InternalNode
            {
            public:
                static const uint8_t MAGIC_BYTE = 0x02;

                virtual ~ExtendedNode() throw(); //!< destructor
                virtual  Node   *     clone() const;
                void                  viz( ios::ostream &) const; //!< graphViz
                virtual const string *data() const throw();

            private:
                Y_DISABLE_ASSIGN(ExtendedNode);
                ExtendedNode(const Rule &, const string &s);
                ExtendedNode( const ExtendedNode &node ) throw();
                friend class Node;
                virtual void emit( ios::ostream & ) const;

                Data shared;
            };

        }
    }
}

#endif

