#include "y/jive/pattern/leading.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jive {

        Leading:: ~Leading() throw()
        {
            assert(0==prev);
            assert(0==next);
            _bzset(kind);
            _bzset(code);
            _bzset(lower);
            _bzset(upper);
            
        }
        
        Leading::Leading(const uint8_t c) throw() :
        Object(),
        next(0),
        prev(0),
        kind(Byte),
        code(c),
        lower(0),
        upper(0)
        {
        }
        
        Leading::Leading(const uint8_t lo, const uint8_t up) throw() :
        Object(),
        next(0),
        prev(0),
        kind(Tier),
        code(0),
        lower(lo),
        upper(up)
        {
            assert(lower<upper);
        }
        
        Leading:: Leading(const Leading &other) throw() :
        Object(),
        next(0),
        prev(0),
        kind(other.kind),
        code(other.code),
        lower(other.lower),
        upper(other.upper)
        {
        }
        
        
        std::ostream & operator<<(std::ostream &os, const Leading &l)
        {
            switch(l.kind)
            {
                case Leading::Byte:
                    os << cchars::visible[l.code];
                    break;
                    
                case Leading::Tier:
                    os << cchars::visible[l.lower] << '-' << cchars::visible[l.upper];
                    break;
            }
            return os;
        }
        
        bool Leading:: owns(const char c) const throw()
        {
            switch(kind)
            {
                case Byte: return c == code;
                case Tier: return lower<=c && c<=upper;
                default:
                    break;
            }
            // never get here
            return false;
        }

        OwnerShip Leading:: whose(const char c) const throw()
        {
            switch(kind)
            {
                case Byte:
                    if(c<code)
                    {
                        return OwnedByPrev;
                    }
                    else if(code<c)
                    {
                        return OwnedByNext;
                    }
                    else
                    {
                        assert(owns(c));
                        return OwnedByThis;
                    }
                    
                case Tier:
                    if(c<lower)
                    {
                        return OwnedByPrev;
                    }
                    else if(upper<c)
                    {
                        return OwnedByNext;
                    }
                    else
                    {
                        assert(owns(c));
                        return OwnedByThis;
                    }
                default:
                    break;
            }
            //never get here
            return OwnedByNext;
        
        }
        
#define YJL_STATUS(A,B) ( (A<<Leading::SHL) | B )
        Leading *Leading:: TryMerge(const Leading *lhs, const Leading *rhs)
        {
            assert(lhs);
            assert(rhs);
            switch( YJL_STATUS(lhs->kind,rhs->kind) )
            {
                case YJL_STATUS(Byte,Byte):
                    assert(lhs->code<rhs->code);
                    if(lhs->code+1==rhs->code)
                    {
                        return new Leading(lhs->code,rhs->code);
                    }
                    break;
                    
                case YJL_STATUS(Byte,Tier):
                    assert(lhs->code<rhs->lower);
                    if(lhs->code+1==rhs->lower)
                    {
                        return new Leading(lhs->code,rhs->upper);
                    }
                    break;
                    
                case YJL_STATUS(Tier,Byte):
                    assert(lhs->upper<rhs->code);
                    if(lhs->upper+1==rhs->code)
                    {
                        return new Leading(lhs->lower,rhs->code);
                    }
                    break;
                    
                case YJL_STATUS(Tier,Tier):
                    assert(lhs->upper<rhs->lower);
                    if(lhs->upper+1==rhs->lower)
                    {
                        return new Leading(lhs->lower,rhs->upper);
                    }
                    break;
                    
                default:
                    assert( die("never get here") );
                    break;
            }
            return NULL;
        }
        
        LeadingChars:: LeadingChars() throw() :
        size(0),
        lead()
        {
        }
        
        LeadingChars:: ~LeadingChars() throw()
        {
            _bzset(size);
        }
        
        
        
        std::ostream & operator<<( std::ostream &os, const LeadingChars &lc)
        {
            os << '{';
            if(lc.lead.size>0)
            {
                const Leading *node = lc.lead.head;
                os << *node;
                for(node=node->next;node;node=node->next)
                {
                    os << ' ' << *node;
                }
            }
            os << '}' << '#' << lc.size << "/" << lc.lead.size;
            return os;
        }
        
        void LeadingChars:: insert(const uint8_t c)
        {
            switch(size)
            {
                case 0:
                    assert(0==lead.size);
                    lead.push_back( new Leading(c) );
                    ++aliasing::_(size);
                    return;
                    
                case 256:
                    //assert(1==lead.size);
                    return;
                    
                default:
                    break;
            }
            assert(size>0);
            assert(size<256);
            assert(lead.size>0);
            
            //------------------------------------------------------------------
            // check if before first node
            //------------------------------------------------------------------
            Leading *curr = lead.head;
            switch(curr->whose(c))
            {
                case OwnedByThis: return; // already inserted
                case OwnedByNext: break;  // after head
                case OwnedByPrev: {
                    const Leading *lhs = lead.push_front( new Leading(c) );
                    Leading       *mrg = Leading::TryMerge(lhs,curr);
                    if(mrg)
                    {
                        assert(lead.size>=2);
                        delete lead.pop_front();
                        delete lead.pop_front();
                        lead.push_front(mrg);
                    }
                    ++aliasing::_(size);
                } return;
            }
            
            
            //------------------------------------------------------------------
            // bracket
            //------------------------------------------------------------------
            assert( curr==lead.head );
            assert( ! curr->owns(c) );
            Leading *next = curr->next;
            while(next)
            {
                switch(next->whose(c))
                {
                    case OwnedByThis: return; // already inserted
                    case OwnedByNext: break;  // step forward
                        
                    case OwnedByPrev:
                    {
                        Leading *node = lead.insert_after(curr,new Leading(c));
                        assert(node->prev==curr);
                        assert(curr->next==node);
                        assert(node->next==next);
                        assert(next->prev==node);
                        ++aliasing::_(size);
                    } return;
                }
                curr = next;
                next = next->next;
            }

            assert(curr==lead.tail);
            {
                const Leading *rhs = lead.push_back( new Leading(c) );
                Leading       *mrg = Leading::TryMerge(curr,rhs);
                if(mrg)
                {
                    assert(lead.size>=2);
                    delete lead.pop_back();
                    delete lead.pop_back();
                    lead.push_back(mrg);
                }
                ++aliasing::_(size);
            }
            
        }

        
    }
    
}

