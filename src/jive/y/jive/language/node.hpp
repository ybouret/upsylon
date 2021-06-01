
#ifndef Y_JIVE_LANG_NODE_INCLUDED
#define Y_JIVE_LANG_NODE_INCLUDED 1

#include "y/jive/lexer.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace Jive
    {

        namespace Language
        {

            class Axiom;

            //__________________________________________________________________
            //
            //
            //! flexible Node for Terminal/Internal
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
                typedef Lexeme::Pointer        Lptr;    //!< alias
                static  const char             CLID[];  //!< "LNode"
                static  const uint8_t          TerminalMarker = 'T'; //!< to write terminals
                static  const uint8_t          InternalMarker = 'I'; //!< to write internals


                //! kind of node
                enum State
                {
                    IsTerminal, //!< a terminal, got a lexeme
                    IsInternal  //!< an internal, got child(ren)
                };

                //______________________________________________________________
                //
                // Serializable
                //______________________________________________________________
                virtual const char *className()       const throw(); //!< CLID
                virtual size_t      serialize(ios::ostream &) const; //!< serialize

                //______________________________________________________________
                //
                // memory methods
                //______________________________________________________________
                static Node *  Acquire(const Axiom &, Lexeme *);     //!< new terminal node
                static Node *  Acquire(const Axiom &);               //!< new internal node
                static void    Release(Node *)           throw();    //!< release memory
                static void    ReturnTo(Lexer &, Node *) throw();    //!< return node to lexer

                //______________________________________________________________
                //
                // utility methods
                //______________________________________________________________
                const Lexeme *lexeme()     const throw(); //!< IsTerminal : content()
                const List   &leaves()     const throw(); //!< IsInternal : List()
                List         &leaves()           throw(); //!< IsInternal : List()

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Axiom &axiom; //!< referenced axiom
                const State  state; //!< internal/terminal
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
                virtual     ~Node() throw();
                explicit     Node(const Axiom &) throw();           //!< a new internal node
                explicit     Node(const Axiom &, Lexeme *) throw(); //!< a new terminal node
                Lptr  &     _Lptr() const throw(); //!< if IsTerminal
                List  &     _List() const throw(); //!< if IsInternal
                void         init() throw();       //!< clear workspace
                virtual void vizCore(ios::ostream &fp) const;

                friend class memory::magazine<Node>;
                uint64_t     wksp[ Y_U64_FOR_ITEM(List) ];
            };

        }

        typedef Language::Node LNode; //!< alias
        typedef LNode::Pointer LTree; //!< alias

    }
}

#endif
