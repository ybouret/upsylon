//! \file
#ifndef Y_JIVE_SYNTAX_NODE_INCLUDED
#define Y_JIVE_SYNTAX_NODE_INCLUDED 1

#include "y/jive/lexer.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace Jive
    {
        
        namespace Syntax
        {
            class Axiom;    //!< forward declaration
            class Terminal; //!< forward declaration
            class Internal; //!< forward declaration

            //__________________________________________________________________
            //
            //
            //! flexible syntax node
            //
            //__________________________________________________________________
            class Node : public inode<Node>
            {
            public:
                enum Kind
                {
                    IsTerminal,
                    IsInternal
                };
                //! base class for a list of code
                typedef core::list_of<Node> ListType;

                class List : public ListType
                {
                public:
                    explicit List() throw();
                    virtual ~List() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };

                const Axiom &axiom;
                const Kind   kind;

                static Node *  Acquire(const Terminal &, Lexeme *lx); //!< new terminal node
                static Node *  Acquire(const Internal &);             //!< new internal node
                static void    Release(Node *node) throw();           //!< release memory
                const Lexeme * lexeme()   const    throw();           //!< access lexeme
                List         & children()          throw();           //!< access children
                const List   & children() const    throw();           //!< access children

                static void    ReturnTo(Lexer &lexer, Node *node) throw(); //!< 

            private:
                explicit Node(const Internal&) throw();
                explicit Node(const Terminal&, Lexeme *lx) throw();
                virtual ~Node() throw();
                void     setup() throw();

                class Lx
                {
                public:
                    Lx(Lexeme *) throw();
                    ~Lx() throw();
                    Lexeme  *lexeme;
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Lx);
                };
                uint64_t wksp[Y_U64_FOR_ITEM(List)];

                Lx   & lptr() const throw();
                List & chld() const throw();


            public:
                typedef memory::tight::supply_of<Node> SupplyType; //!< alias

                //! todo
                class Supply : public singleton<Supply>, public SupplyType
                {
                public:
                    Y_SINGLETON_DECL(Supply);

                    void  release(Node *) throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Supply);
                    explicit Supply();
                    virtual ~Supply() throw();
                    friend class Node;
                };
            };
            
        }

        typedef Syntax::Node  XNode;
    }
}
    

#endif
