//! \file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED 1

#include "y/lang/lexical/lexer.hpp"
#include "y/ptr/arc.hpp"
#include "y/lang/pattern/matching.hpp"

namespace upsylon
{
    namespace Lang
    {

        namespace Syntax
        {
            class Rule;      //!< forward declaration
            class Grammar;   //!< forward declaration

            //! store syntax trees
            class Node : public Object, public core::inode<Node>, public Vizible
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
                virtual const void   *inner() const throw()       = 0; //!< internal address for lexeme/children
                virtual void          returnTo( Lexer & ) throw() = 0; //!< restore before delete
                virtual const string *data() const throw()        = 0; //!< return extra data, if any
                virtual              ~Node() throw();                  //!< destructor
                
                
                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                Lexeme       &lexeme() throw();                                   //!< from inner
                const Lexeme &lexeme() const throw();                             //!< from inner
                List         &children() throw();                                 //!< from inner
                const List   &children() const throw();                           //!< from inner
                void          save( ios::ostream &fp, size_t *bytes=0) const;     //!< save to stream
                void          save( const string &binfile, size_t *bytes=0) const;//!< save to file
                void          save( const char   *binfile, size_t *bytes=0) const;//!< save to file
                size_t        outputBytes() const;                                //!< count the bytes for output
                string        toBinary() const;                                   //!< to a binary string
                string        toBase64() const;                                   //!< to a human readable string

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

                //! wrapper to use module
                static Node  *Load( Module *m, Grammar &G);

                //! transform a raw node into the direct AST
                /**
                 remove univocal content and semantic node,
                 and apply node behavior to merge possible branches
                 */
                static Node * AST( Node *node ) throw();

                //! rewrite a standalone node from AST
                /**
                 transform using operators, a second pass AST
                 should be called
                 */
                static Node * Rewrite( Node *node, const Grammar &G );

                //! Compact only the AST
                /**
                 apply only node behavior to merge branche after
                 a Rewrite operation, does not touch terminals
                 */
                static Node * Compact( Node *node ) throw();

                //! remove some nodes matching the expression
                static void RemoveFrom( Node &node, Matching &name_matches );



            protected:
                explicit Node(const Rule &r, const bool term) throw(); //!< setup
                Node(const Node &other) throw();                       //!< copy

            private:
                Y_DISABLE_ASSIGN(Node);
                virtual void        emit( ios::ostream & , size_t *) const = 0;

            };

            //! a Terminal Node, acts as a lexeme smart pointer
            class TerminalNode : public Node
            {
            public:
                static const uint8_t MAGIC_BYTE = 0x00;            //!< for I/O

                virtual ~TerminalNode() throw();                       //!< destructor
                virtual Node       *  clone() const;                   //!< clone
                virtual const void *  inner() const throw();           //!< lx
                virtual void          vizCore( ios::ostream & ) const; //!< for graphViz
                virtual const string *data() const throw();            //!< NULL

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TerminalNode);
                Lexeme *lx;

                explicit TerminalNode(const Rule &r, Lexeme *l) throw();
                virtual void emit( ios::ostream &, size_t *) const;
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
                void                  vizCore( ios::ostream & ) const; //!< graphViz
                virtual const string *data() const throw();        //!< data or NULL

            protected:
                InternalNode(const InternalNode &) throw();             //!< copy
                InternalNode(const Rule &r) throw();                    //!< from rule
                virtual void emit( ios::ostream &, size_t *) const;     //!< emit binary
                void         emitList( ios::ostream &, size_t *) const; //!< emit list of children
                void         vizLink( ios::ostream & ) const;           //!< emit links

            private:
                friend class Node;
                Y_DISABLE_ASSIGN(InternalNode);
                virtual void returnTo( Lexer &lexer ) throw();
                
            };

            //! an Extended Node is an internal node with data, for variable operator
            class ExtendedNode : public InternalNode
            {
            public:
                static const uint8_t MAGIC_BYTE = 0x02;           //!< for I/O

                virtual ~ExtendedNode() throw();                  //!< destructor
                virtual  Node   *     clone() const;              //!< hardcopy with shared data
                void                  vizCore( ios::ostream &) const; //!< graphViz
                virtual const string *data() const throw();       //!< & *shared

            private:
                Y_DISABLE_ASSIGN(ExtendedNode);
                ExtendedNode(const Rule &, const string &s);
                ExtendedNode( const ExtendedNode &node ) throw();
                friend class Node;
                virtual void emit( ios::ostream &, size_t *bytes) const;

                Data shared;
            };

        }
    }
}

#endif

