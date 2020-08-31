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
        
        
        size_t Leading:: count() const throw()
        {
            switch(kind)
            {
                case Byte: return 1;
                case Tier: return 1+size_t(upper)-size_t(lower);
                default:
                    break;
            }
            // never get here
            return 0;
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
        
        bool Leading:: owns(const uint8_t c) const throw()
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

        OwnerShip Leading:: whose(const uint8_t c) const throw()
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
        
#define YJL_STATUS(A,B) ( ( unsigned(A)<<Leading::SHL) | unsigned(B) )
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
        
        bool Leading::AreApart(const Leading *lhs, const Leading *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            switch( YJL_STATUS(lhs->kind,rhs->kind) )
            {
                case YJL_STATUS(Byte,Byte):
                    assert(lhs->code<rhs->code);
                    return rhs->code-lhs->code>1;
                    
                case YJL_STATUS(Byte,Tier):
                    assert(lhs->code<rhs->lower);
                    return rhs->lower-lhs->code>1;
                    
                    
                case YJL_STATUS(Tier,Byte):
                    assert(lhs->upper<rhs->code);
                    return rhs->code-lhs->upper>1;
                    
                    
                case YJL_STATUS(Tier,Tier):
                    assert(lhs->upper<rhs->lower);
                    return rhs->lower-rhs->upper>1;
                    
                default:
                    assert( die("never get here") );
                    break;
            }
            return true;
        }

        
        void Leading::Compact3(Leading::List &L, Leading *a, Leading *b, Leading *c)
        {
            Leading *ab = TryMerge(a,b);
            if(ab)
            {
                delete L.unlink(a);
                delete L.replace(b,ab);
                assert(ab->next==c);
                assert(c->prev==ab);
                Leading *abc = TryMerge(ab,c);
                if(abc)
                {
                    delete L.unlink(ab);
                    delete L.replace(c,abc);
                }
            }
            else
            {
                // a and b are disjoint
                Leading *bc = TryMerge(b,c);
                if(bc)
                {
                    delete L.unlink(b);
                    delete L.replace(c,bc);
                }
            }
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
            assert(check());
            
            //------------------------------------------------------------------
            // study status
            //------------------------------------------------------------------
            switch(size)
            {
                case 0:
                    assert(0==lead.size);
                    lead.push_front( new Leading(c) );
                    ++aliasing::_(size);
                    assert( check() );
                    assert(1==size);
                    assert(1==lead.size);
                    assert(lead.head->owns(c));
                    return;
                    
                case 256:
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
                case OwnedByThis: assert(curr->owns(c)); return; // already inserted
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
                    else
                    {
                        assert( Leading::AreApart(lhs,curr) );
                    }
                    ++aliasing::_(size);
                    assert( check() );
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
                    case OwnedByThis: assert(next->owns(c)); return; // already inserted
                    case OwnedByNext: break;  // step forward
                        
                    case OwnedByPrev:
                    {
                        Leading *node = lead.insert_after(curr,new Leading(c));
                        assert(node->prev==curr);
                        assert(curr->next==node);
                        assert(node->next==next);
                        assert(next->prev==node);
                        Leading::Compact3(lead,curr,node,next);
                        ++aliasing::_(size);
                        assert(check());
                    } return;
                }
                curr = next;
                next = next->next;
            }
            
            //------------------------------------------------------------------
            // append after tail
            //------------------------------------------------------------------
            
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
                else
                {
                    assert( Leading::AreApart(curr,rhs) );
                }
                ++aliasing::_(size);
                assert( check() );
            }
            
        }

       
        void LeadingChars:: release() throw()
        {
            lead.release();
            aliasing::_(size) = 0;
            assert(lead.size<=0);
        }
        
        void LeadingChars:: complete()
        {
            release();
            lead.push_back( new Leading(0x00,0xff) );
            aliasing::_(size) = 1;
        }
        
        void LeadingChars:: remove(const uint8_t c)
        {
            
        }

        
    }
    
}

#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jive {
        
        bool LeadingChars:: check() const
        {
            size_t ans = 0;
            for(const Leading *node=lead.head;node;node=node->next)
            {
                ans += node->count();
            }
            if(ans!=size)
            {
                throw exception("found #%lu instead of %lu", (unsigned long)ans, (unsigned long)size );
            }
            return true;
        }
        
    }
}
