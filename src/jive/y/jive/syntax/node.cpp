
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
            
            void Node::Supply:: reserve(const size_t n)
            {
                Y_LOCK(access);
                fetch(n);
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
                setup();
            }
            
            void Node:: setup() throw()
            {
                memset(wksp,0,sizeof(wksp));
            }
            
            
            Node:: Node(const Axiom &term, Lexeme *lx) throw() :
            axiom(term),
            kind(IsTerminal),
            wksp()
            {
                setup();
                new ( aliasing::anonymous(wksp) ) Lptr(lx);
            }
            
            Node:: Node(const Axiom &in) throw() :
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
            
            Node * Node:: Acquire(const Axiom &term, Lexeme *lx)
            {
                auto_ptr<Lexeme>  guard(lx);
                static Supply    &mgr  = Supply::instance();
                return new(mgr.zquery()) Node(term,guard.yield());
            }
            
            
            Node * Node::Acquire(const Axiom &in)
            {
                static Supply    &mgr = Supply::instance();
                return new(mgr.zquery()) Node(in);
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
            
            void Node:: vizCore(ios::ostream &fp) const
            {
                
                switch (kind)
                {
                    case IsTerminal:
                        fp << "[label=\"" << *axiom.name << "\",shape=box];";
                        break;
                        
                    case IsInternal:
                        fp << "[label=\"" << *axiom.name << "\"];";
                        for(const Node *node=leaves().head;node;node=node->next) {
                            node->vizSave(fp);
                            endl(vizJoin(fp,node));
                        }  break;
                }
                
            }
            
           
            
        }
        
    }
    
}

