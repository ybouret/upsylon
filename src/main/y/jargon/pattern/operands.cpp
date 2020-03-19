
#include "y/jargon/pattern/operands.hpp"


namespace upsylon {
    
    namespace Jargon {

        Operands:: ~Operands() throw()
        {
        }
        
        Operands::  Operands() throw() : Pattern::List() {}
        
        Operands:: Operands( const Operands &other ) : Pattern::List(other)
        {
        }
        
        bool Operands::Alike( const Operands *lhs, const Operands *rhs ) throw()
        {
            assert(lhs);
            assert(rhs);
            if(lhs->size==rhs->size)
            {
                for(const Pattern *p=lhs->head,*q=rhs->head;p;p=p->next,q=q->next)
                {
                    if( !p->alike(q) ) return false;
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void Operands:: removeRedondant() throw()
        {
            Pattern::List tmp;
            while(size)
            {
                Pattern *p     = pop_front();
                bool    found = false;
                for(const Pattern *q=tmp.head;q;q=q->next)
                {
                    if( p->alike(q) )
                    {
                        found = true;
                        break;
                    }
                }
                if(found)
                {
                    delete p;
                }
                else
                {
                    tmp.push_back(p);
                }
            }
            swap_with(tmp);
        }


        
    }
}

