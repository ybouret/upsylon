
#include "y/jive/language/node.hpp"

namespace upsylon
{

    Y_MAGAZINE_IMPL(Jive::LNode::Supply, Jive::Lexical::Unit::Supply::life_time - 1);

    namespace Jive
    {
        namespace Language
        {

            const char Node:: CLID[] = "LNode";


            Node::Lptr  & Node:: _Lptr() const throw()
            {
                assert(IsTerminal==state);
                return *static_cast<Lptr *>( (void *)aliasing::anonymous(wksp) );
            }

            Node::List   & Node:: _List() const throw()
            {
                assert(IsInternal==state);
                return *static_cast<List *>( (void*)aliasing::anonymous(wksp) );
            }


            Node:: ~Node() throw()
            {
                switch(state)
                {
                    case IsInternal:
                        collapse(_List());
                        break;

                    case IsTerminal:
                        collapse(_Lptr());
                        break;
                }
                init();
            }

            void Node:: init() throw()
            {
                memset(wksp,0,sizeof(wksp));
            }

            Node:: Node(const Axiom &a) throw() :
            inode<Node>(),
            axiom(a),
            state(IsInternal),
            wksp()
            {
                init();
                new ( aliasing::anonymous(wksp) ) List();
            }

            Node:: Node(const Axiom &a, Lexeme *l) throw() :
            inode<Node>(),
            axiom(a),
            state(IsTerminal),
            wksp()
            {
                assert(NULL!=l);
                init();
                new ( aliasing::anonymous(wksp) ) Lptr(l);
            }


            void Node:: Release(Node *node) throw()
            {
                assert(node);
                assert(Supply::exists());
                static Supply &mgr = Supply::location();
                mgr.release(node);
            }

            Node * Node:: Acquire(const Axiom &term, Lexeme *lx)
            {
                Lexeme::Pointer   guard(lx);
                static Supply    &mgr  = Supply::instance();
                Node * node = mgr.acquire<const Axiom&,Lexeme *>(term,lx);
                guard.dismiss();
                return node;
            }


            Node * Node::Acquire(const Axiom &in)
            {
                static Supply    &mgr = Supply::instance();
                return mgr.acquire<const Axiom&>(in);
            }

            void Node:: ReturnTo(Lexer &lexer, Node *node) throw()
            {
                assert(node);
                switch(node->state)
                {
                    case IsTerminal: {
                        Lptr & lx = node->_Lptr(); assert(lx.is_valid());
                        lexer.unget(lx.yield());
                    } break;

                    case IsInternal: {
                        List &L = node->_List();
                        while(L.size)
                        {
                            ReturnTo(lexer,L.pop_back());
                        }
                    } break;
                }
                Release(node);
            }

           

        }
    }
}

