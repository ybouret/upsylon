//! \file
#ifndef Y_JIVE_SYNTAX_NODE_INCLUDED
#define Y_JIVE_SYNTAX_NODE_INCLUDED 1

#include "y/jive/lexer.hpp"
#include "y/code/round.hpp"
#include "y/ptr/ptr.hpp"

namespace upsylon
{

    namespace Jive
    {
        
        namespace Syntax
        {
            class Axiom;    //!< forward declaration
            class Grammar;  //!< forward declaration
            
            //__________________________________________________________________
            //
            //
            //! flexible syntax node
            //
            //__________________________________________________________________
            class Node : public inode<Node>, public Vizible, public Serializable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef memory::magazine<Node> Supply;  //!< alias
                typedef Supply::list_          List;    //!< alias
                typedef Supply::auto_ptr       Pointer; //!< alias
                
                //! kind of node
                enum Kind
                {
                    IsTerminal, //!< a terminal, got a lexeme
                    IsInternal  //!< an internal, got child(ren)
                };
                


                static const char    CLID[];                //!< "XNode"
                static const uint8_t TerminalMarker = 0x00; //!< for serialize
                static const uint8_t InternalMarker = 0x01; //!< for serialize

                //______________________________________________________________
                //
                // serializable
                //______________________________________________________________
                virtual size_t      serialize(ios::ostream &) const;
                virtual const char *className() const throw(); //!< CLID
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const Lexeme * lexeme()     const throw();         //!< access lexeme
                List         & leaves()           throw();         //!< access leaves
                const List   & leaves()     const throw();         //!< access leaves
                bool           isInternal() const throw();         //!< kind==IsInternal
                bool           isTerminal() const throw();         //!< kind==IsTerminal
                const string  &name()       const throw();         //!< axiom.name
                
                template <typename ID> inline
                bool is(const ID &id) const throw()
                {
                    return id == name();
                }

                template <typename ID> inline
                bool isTerminal(const ID &id) const throw()
                {
                    return isTerminal() && is(id);
                }

                template <typename ID> inline
                bool isInternal(const ID &id) const throw()
                {
                    return isInternal() && is(id);
                }




                //______________________________________________________________
                //
                // static methods
                //______________________________________________________________
                static Node *  Acquire(const Axiom &, Lexeme *);     //!< new terminal node
                static Node *  Acquire(const Axiom &);               //!< new internal node
                static Supply &Instance();                           //!< instance
                static void    Release(Node *)           throw();    //!< release memory
                static void    ReturnTo(Lexer &, Node *) throw();    //!< return node to lexer
                static Node *  Load(Source        &source,
                                    const Grammar &G,
                                    size_t        &maxLength);       //!< reload a node

                //! reload a node from a file
                template <typename FILENAME> static inline
                Node * LoadFile(const FILENAME &fileName,
                                const Grammar  &G,
                                size_t         &maxLength)
                {
                    Source source( Module::OpenFile(fileName) );
                    return Load(source,G,maxLength);
                }

                //! build the AST
                static Node *  AST(Node *node) throw();

                //! regularize lexeme stamps, return max unit length
                size_t regularized() throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Axiom &axiom; //!< creator
                const Kind   kind;  //!< kind

            private:
                explicit     Node(const Axiom&) throw();
                explicit     Node(const Axiom&, Lexeme *lx) throw();
                virtual     ~Node()  throw();
                void         setup() throw();
                virtual void vizCore(ios::ostream &) const;
                friend class memory::magazine<Node>;


                typedef Lexeme::Pointer Lptr;
                
                Lptr & _Lptr() const throw(); //!< if terminal
                List & _List() const throw(); //!< if internal
                
                uint64_t wksp[Y_U64_FOR_ITEM(List)]; //!< memory
                static Node *AST_Internal(Node *) throw();
                static Node *AST_Terminal(Node *) throw();

                void regularize(size_t &stamp, size_t &length) throw();
                static Node *Reload(Source &source,const Grammar &); //!< reload a node
            };
            
        }

        typedef Syntax::Node  XNode; //!< alias
        typedef XNode::List   XList; //!< alias
    }

	Y_MAGAZINE_DECL(Jive::Syntax::Node)

}
    

#endif
