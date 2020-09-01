#include "y/jive/pattern/leading.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {
    
    namespace Jive {

        Interval:: ~Interval() throw()
        {
            assert(0==prev);
            assert(0==next);
            _bzset(lower);
            _bzset(upper);
            
        }
        
        Interval::Interval(const uint8_t c) throw() :
        Object(),
        next(0),
        prev(0),
        lower(c),
        upper(c)
        {
        }
        
        Interval::Interval(const uint8_t lo, const uint8_t up) throw() :
        Object(),
        next(0),
        prev(0),
        lower(lo),
        upper(up)
        {
        }
        
        
        
        size_t Interval:: width() const throw()
        {
            return 1+size_t(upper)-size_t(lower);
        }

        std::ostream & operator<<(std::ostream &os, const Interval &l)
        {
            assert(l.lower<=l.upper);
            if(l.lower<l.upper)
            {
                os << cchars::visible[l.lower] << '-' << cchars::visible[l.upper];
            }
            else
            {
                os << cchars::visible[l.lower];
            }

            return os;
        }
        
        bool Interval:: owns(const uint8_t c) const throw()
        {

            return lower<=c && c<=upper;
        }

        OwnerShip Interval:: whose(const uint8_t c) const throw()
        {

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


        }
        
        Interval *Interval:: TryMerge(const Interval *lhs, const Interval *rhs)
        {
            assert(lhs);
            assert(rhs);

            assert(lhs->upper<rhs->lower);
            if(lhs->upper+1==rhs->lower)
            {
                return new Interval(lhs->lower,rhs->upper);
            }
            else
            {
                return NULL;
            }

        }
        
        bool Interval::AreApart(const Interval *lhs, const Interval *rhs) throw()
        {
            assert(lhs->upper<rhs->lower);
            return lhs->upper+1<rhs->lower;
        }

        
        void Interval::Compact3(List &L, Interval *a, Interval *b, Interval *c)
        {
            Interval *ab = TryMerge(a,b);
            if(ab)
            {
                delete L.unlink(a);
                delete L.replace(b,ab);
                assert(ab->next==c);
                assert(c->prev==ab);
                Interval *abc = TryMerge(ab,c);
                if(abc)
                {
                    delete L.unlink(ab);
                    delete L.replace(c,abc);
                }
            }
            else
            {
                // a and b are disjoint
                Interval *bc = TryMerge(b,c);
                if(bc)
                {
                    delete L.unlink(b);
                    delete L.replace(c,bc);
                }
            }
        }
        
        Leading:: Leading() throw() :
        size(0),
        parts()
        {
        }
        
        Leading:: ~Leading() throw()
        {
            _bzset(size);
        }
        
        
        
        std::ostream & operator<<( std::ostream &os, const Leading &lc)
        {
            os << '{';
            if(lc.parts.size>0)
            {
                const Interval *node = lc.parts.head;
                os << *node;
                for(node=node->next;node;node=node->next)
                {
                    os << ' ' << *node;
                }
            }
            os << '}' << '#' << lc.size << "/" << lc.parts.size;
            return os;
        }
        
        bool Leading:: insert(const uint8_t c)
        {
            assert(check());
            
            //------------------------------------------------------------------
            // study status
            //------------------------------------------------------------------
            switch(size)
            {
                case 0:
                    assert(0==parts.size);
                    parts.push_front( new Interval(c) );
                    ++aliasing::_(size);
                    assert( check() );
                    assert(1==size);
                    assert(1==parts.size);
                    assert(parts.head->owns(c));
                    return true;
                    
                case 256:
                    return false;
                    
                default:
                    break;
            }
            assert(size>0);
            assert(size<256);
            assert(parts.size>0);
            
            //------------------------------------------------------------------
            // check if before first node
            //------------------------------------------------------------------
            Interval *curr = parts.head;
            switch(curr->whose(c))
            {
                case OwnedByThis: assert(curr->owns(c)); return false; // already inserted
                case OwnedByNext: break;  // after head
                case OwnedByPrev: {
                    const Interval *lhs = parts.push_front( new Interval(c) );
                    Interval       *mrg = Interval::TryMerge(lhs,curr);
                    if(mrg)
                    {
                        assert(parts.size>=2);
                        delete parts.pop_front();
                        delete parts.pop_front();
                        parts.push_front(mrg);
                    }
                    else
                    {
                        assert( Interval::AreApart(lhs,curr) );
                    }
                    ++aliasing::_(size);
                    assert( check() );
                } return true;
            }
            
            
            //------------------------------------------------------------------
            // bracket
            //------------------------------------------------------------------
            assert( curr==parts.head );
            assert( ! curr->owns(c) );
            Interval *next = curr->next;
            while(next)
            {
                switch(next->whose(c))
                {
                    case OwnedByThis: assert(next->owns(c)); return false; // already inserted
                    case OwnedByNext: break;  // step forward
                        
                    case OwnedByPrev:
                    {
                        Interval *node = parts.insert_after(curr,new Interval(c));
                        assert(node->prev==curr);
                        assert(curr->next==node);
                        assert(node->next==next);
                        assert(next->prev==node);
                        Interval::Compact3(parts,curr,node,next);
                        ++aliasing::_(size);
                        assert(check());
                    } return true;
                }
                curr = next;
                next = next->next;
            }
            
            //------------------------------------------------------------------
            // append after tail
            //------------------------------------------------------------------
            
            assert(curr==parts.tail);
            {
                const Interval *rhs = parts.push_back( new Interval(c) );
                Interval       *mrg = Interval::TryMerge(curr,rhs);
                if(mrg)
                {
                    assert(parts.size>=2);
                    delete parts.pop_back();
                    delete parts.pop_back();
                    parts.push_back(mrg);
                }
                else
                {
                    assert( Interval::AreApart(curr,rhs) );
                }
                ++aliasing::_(size);
                assert( check() );
            }
            return true;
        }


        void Leading:: release() throw()
        {
            parts.release();
            aliasing::_(size) = 0;
            assert(parts.size<=0);
        }
        
        void Leading:: complete()
        {
            release();
            parts.push_back( new Interval(0x00,0xff) );
            aliasing::_(size) = 256;
        }
        
        bool Leading:: remove(const uint8_t c)
        {
            if(parts.size>0)
            {
                for(Interval *node=parts.head;node;node=node->next)
                {
                    switch(node->whose(c))
                    {
                        case OwnedByPrev: return false; //!< too low or already checked
                        case OwnedByThis: return removeFrom(node,c);
                        case OwnedByNext: break;
                    }
                }
                return false; // too high
                
            }
            else
            {
                return false;
            }
        }
        
        bool Leading:: removeFrom(Interval *node, const uint8_t c)
        {
            assert(node);
            assert(node->owns(c));
            const unsigned num_lower = c-node->lower;
            const unsigned num_upper = node->upper-c;
            if(num_lower<=0)
            {
                if(num_upper<=0)
                {
                    //----------------------------------------------------------
                    // num_lower=0 and num_upper=0
                    //----------------------------------------------------------
                    delete parts.unlink(node);
                }
                else
                {
                    //----------------------------------------------------------
                    // num_lower=0 and num_upper>0
                    //----------------------------------------------------------
                    assert(num_upper>0);
                    ++aliasing::_(node->lower);
                }
            }
            else
            {
                assert(num_lower>0);
                if(num_upper<=0)
                {
                    //----------------------------------------------------------
                    // num_lower>0 and num_upper<=0
                    //----------------------------------------------------------
                    --aliasing::_(node->upper);
                }
                else
                {
                    assert(num_upper>0);
                    //----------------------------------------------------------
                    // num_upper>0 and num_lower>0
                    //----------------------------------------------------------
                    const uint8_t lhs_upper = c-1;
                    assert(node->lower<=lhs_upper);

                    const uint8_t rhs_lower = c+1;
                    const uint8_t rhs_upper = node->upper;
                    assert(rhs_lower<=rhs_upper);
                    parts.insert_after(node, new Interval(rhs_lower,rhs_upper) );
                    aliasing::_(node->upper) = lhs_upper;
                }
            }

            --aliasing::_(size);
            return true;
        }

        size_t Leading:: insert(const uint8_t lo,const uint8_t hi)
        {
            unsigned lower=lo,upper=hi;
            if(lower>upper) cswap(lower,upper);
            size_t n = 0;
            for(size_t i=lower;i<=upper;++i)
            {
                if(insert(uint8_t(i))) ++n;
            }
            return n;
        }

        void Leading:: include(const Leading &other)
        {
            if(this!=&other)
            {
                for(const Interval *node=other.parts.head;node;node=node->next)
                {
                    const unsigned top = node->upper;
                    for(unsigned i=node->lower;i<=top;++i)
                    {
                        (void) insert( uint8_t(i) );
                    }
                }
            }
        }

        void Leading:: exclude(const Leading &other)
        {
            if(this!=&other)
            {
                for(const Interval *node=other.parts.head;node;node=node->next)
                {
                    const unsigned top = node->upper;
                    for(unsigned i=node->lower;i<=top;++i)
                    {
                        (void) remove( uint8_t(i) );
                    }
                }
            }
            else
            {
                release();
            }
        }

        void Leading:: opposite(const Leading &other)
        {
            Leading tmp;
            tmp.complete();
            tmp.exclude(other);

            _cswap(size,tmp.size);
            tmp.parts.swap_with(parts);
        }

        bool Leading:: search(const uint8_t c) const throw()
        {
            if(parts.size>0)
            {
                const Interval *incr = parts.head;
                const Interval *decr = parts.tail;

            CYCLE:
                switch( incr->whose(c) )
                {
                    case OwnedByPrev: return false; // too low
                    case OwnedByThis: return true;  // OK!
                    case OwnedByNext: break;
                }

                switch( decr->whose(c) )
                {
                    case OwnedByNext: return false; // too high
                    case OwnedByThis: return true;  // OK!
                    case OwnedByPrev: break;
                }

                assert(decr!=incr);
                {
                    const Interval *temp = incr;
                    incr=incr->next;
                    if(incr==decr)
                    {
                        return false; // already test
                    }
                    else
                    {
                        decr=decr->prev;
                        if(decr==temp)
                        {
                            return false;
                        }
                        else
                        {
                            goto CYCLE;
                        }
                    }
                }


            }
            else
            {
                assert(size==0);
                return false;
            }
        }

    }
    
}

#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jive {
        
        bool Leading:: check() const
        {
            size_t ans = 0;
            for(const Interval *node=parts.head;node;node=node->next)
            {
                ans += node->width();
            }
            if(ans!=size)
            {
                throw exception("found #%lu instead of %lu", (unsigned long)ans, (unsigned long)size );
            }
            return true;
        }
        
    }
}

#include "y/jive/source.hpp"

namespace upsylon {

    namespace Jive {

        bool   Source:: find(const Leading &fc)
        {
            //------------------------------------------------------------------
            // look up in cache
            //------------------------------------------------------------------
            while(cache.size>0)
            {
                if( fc.search(cache.head->code) )
                {
                    return true;
                }
                Char::Release( cache.pop_front() );
            }

            //------------------------------------------------------------------
            // look up in new char
            //------------------------------------------------------------------
        PROBE:
            assert(cache.size==0);
            Char *ch = probe();
            if(!ch) return false;
            if( fc.search(ch->code) )
            {
                assert(cache.size==0);
                cache.push_back(ch);
                return true;
            }
            goto PROBE;

        }

    }

}
