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
            class Node :
            public inode<Node>,
            public Vizible,
            public Serializable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                //! alias
                typedef memory::tight::supply_of<Node> SupplyType;
                
                
                //! dedicated supply for Nodes
                class Supply : public singleton<Supply>, public SupplyType
                {
                public:
                    Y_SINGLETON_DECL(Supply);      //!< aliases
                    void  release(Node *) throw(); //!< destruct and store node
                    void  reserve(const size_t);   //!< reserve extra nodes
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Supply);
                    explicit Supply();
                    virtual ~Supply() throw();
                    friend class Node;
                };
                
                //! kind of node
                enum Kind
                {
                    IsTerminal, //!< a terminal, got a lexeme
                    IsInternal  //!< an internal, got child(ren)
                };
                
                //! base class for a list of code
                typedef core::list_of<Node> ListType;

                //! dedicated list for leaves
                class List : public ListType
                {
                public:
                    explicit List() throw(); //!< setup
                    virtual ~List() throw(); //!< cleanup

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };
                
                //! dedicated smart pointer
                class Pointer : public ptr<Node>
                {
                public:
                    explicit Pointer(Node*) throw();
                    virtual ~Pointer() throw();
                    Node    *yield() throw();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Pointer);
                    void zap() throw();
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
                const Lexeme * lexeme() const    throw();         //!< access lexeme
                List         & leaves()          throw();         //!< access leaves
                const List   & leaves() const    throw();         //!< access leaves

                //______________________________________________________________
                //
                // static methods
                //______________________________________________________________
                static Node *  Acquire(const Axiom &, Lexeme *);     //!< new terminal node
                static Node *  Acquire(const Axiom &);               //!< new internal node
                static void    Release(Node *)           throw();    //!< release memory
                static void    ReturnTo(Lexer &, Node *) throw();    //!< return node to lexer
                static Node *  Load(Source &source,const Grammar &); //!< reload a node
                
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
                
                //! lexeme pointer wrapper
                class Lptr
                {
                public:
                    Lptr(Lexeme *) throw();
                    ~Lptr() throw();
                    Lexeme  *lexeme;
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Lptr);
                };
                
                Lptr & _Lptr() const throw(); //!< if terminal
                List & _List() const throw(); //!< if internal
                
                uint64_t wksp[Y_U64_FOR_ITEM(List)]; //!< memory
                
            };
            
        }

        typedef Syntax::Node  XNode; //!< alias
    }
}
    

#endif
