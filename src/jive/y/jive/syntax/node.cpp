


#include "y/jive/syntax/axiom/terminal.hpp"
#include "y/jive/syntax/axiom/internal.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace memory
    {
        Y_SINGLETON_TEMPLATE_WITH(Jive::Lexical::Unit::Supply::life_time-1,Jive::XNode::Supply);

    }
}

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            
            
            
            const Lexeme * Node::lexeme() const throw()
            {
                return _Lptr().content();
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
                switch(node->kind)
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
            
            void Node:: vizCore(ios::ostream &fp) const
            {
                
                switch (kind)
                {
                    case IsTerminal:
                        fp << "[label=\"" << *axiom.name << "='" << lexeme()->toEncoded() << "'\",shape=box];";
                        break;
                        
                    case IsInternal:
                        fp << "[label=\"" << *axiom.name << "\",shape=oval];";
                        for(const Node *node=leaves().head;node;node=node->next) {
                            node->vizSave(fp);
                            endl(vizJoin(fp,node));
                        }  break;
                }
                
            }
            
           
            
        }
        
    }
    
}

