
#include "y/jive/syntax/node.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Y_SINGLETON_IMPL_WITH(Char::Supply::life_time-1,Node::Supply);

            Node::Supply:: Supply()
            {
            }

            Node::Supply:: ~Supply() throw()
            {
            }

            void Node:: Supply:: release(Node *node) throw()
            {
                Y_LOCK(access);
                assert(node);
                node->~Node();
                zstore(node);
            }




        }
    }
}

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            Node:: List:: List() throw() : ListType()
            {
            }

            Node:: List:: ~List() throw()
            {
                while(size)
                {
                    assert(Supply::exists());
                    static Supply &mgr = Supply::location();
                    mgr.release( pop_back() );
                }
            }

        }

    }
}


#include "y/jive/syntax/axiom/terminal.hpp"
#include "y/jive/syntax/axiom/internal.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            Node::Lptr::Lptr(Lexeme *lx) throw() :
            lexeme(lx)
            {
            }

            Node:: Lptr:: ~Lptr() throw()
            {
                if(lexeme)
                {
                    delete lexeme;
                    lexeme = 0;
                }
            }

            
            const Lexeme * Node::lexeme() const throw()
            {
                return _Lptr().lexeme;
            }

            Node::List & Node::leaves() throw()
            {
                return _List();
            }

            const Node::List & Node::leaves() const throw()
            {
                return _List();
            }

            Node::Lptr  & Node:: _Lptr() const throw()
            {
                assert(IsTerminal==kind);
                return *static_cast<Lptr *>( (void *)aliasing::anonymous(wksp) );
            }

            Node::List   & Node:: _List() const throw()
            {
                assert(IsInternal==kind);
                return *static_cast<List *>( (void*)aliasing::anonymous(wksp) );
            }


            Node:: ~Node() throw()
            {
                switch(kind)
                {
                    case IsTerminal:
                        self_destruct(_Lptr());
                        break;

                    case IsInternal:
                        self_destruct(_List());
                        break;
                }
            }

            void Node:: setup() throw()
            {
                memset(wksp,0,sizeof(wksp));
                std::cerr << "setup Node : " << sizeof(wksp) << std::endl;
            }


            Node:: Node(const Terminal &term, Lexeme *lx) throw() :
            axiom(term),
            kind(IsTerminal),
            wksp()
            {
                setup();
                new ( aliasing::anonymous(wksp) ) Lptr(lx);
            }

            Node:: Node(const Internal &in) throw() :
            axiom(in),
            kind(IsInternal),
            wksp()
            {
                setup();
                new ( aliasing::anonymous(wksp) ) List();
            }


            void Node:: Release(Node *node) throw()
            {
                assert(node);
                assert(Supply::exists());
                static Supply &mgr = Supply::location();
                mgr.release(node);
            }

            Node * Node::Acquire(const Terminal &term, Lexeme *lx)
            {
                auto_ptr<Lexeme> guard(lx);
                static Supply    &mgr = Supply::instance();
                Node *node = mgr.zquery();
                return new(node) Node(term,guard.yield());
            }


            Node * Node::Acquire(const Internal &in)
            {
                static Supply    &mgr = Supply::instance();
                Node *node = mgr.zquery();
                return new(node) Node(in);
            }

            void Node:: ReturnTo(Lexer &lexer, Node *node) throw()
            {
                assert(node);
                switch(node->kind)
                {
                    case IsTerminal: {
                        Lptr & lx = node->_Lptr(); assert(lx.lexeme);
                        lexer.unget(lx.lexeme);
                        lx.lexeme = 0;
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

