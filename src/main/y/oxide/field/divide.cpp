
#include "y/oxide/field/divide.hpp"

namespace upsylon
{
    namespace Oxide
    {
        const size_t Divide:: InitialCapacity[4] = { 0, 1, 16, 64 };

        std::ostream & operator<<( std::ostream &os, const Divide::Score &s )
        {
            os << '(' << s.items << ',' << s.async << ',' << s.local << ')';
            return os;
        }
        
        Divide:: Score:: ~Score() throw()
        {
        }
        
        Divide:: Score:: Score(const score_t &_items,
                               const score_t &_async,
                               const score_t &_local) :
        items( _items ),
        async( _async ),
        local( _local )
        {
            
        }
        
        int Divide:: Score:: CompareByDrecreasingValues(const Score *lhs, const Score *rhs, void *) throw()
        {
            if(lhs->items<rhs->items)
            {
                return 1;
            }
            else if(rhs->items<lhs->items)
            {
                return -1;
            }
            else
            {
                if(lhs->async<rhs->async)
                {
                    return 1;
                }
                else if(rhs->async<lhs->async)
                {
                    return -1;
                }
                else
                {
                    return comparison::decreasing(lhs->local, rhs->local);
                }
            }
        }
        
        
    }
}

#include "y/sort/merge.hpp"

namespace upsylon
{
    namespace Oxide
    {
        Divide:: Scores:: ~Scores() throw()
        {
        }
        
        Divide:: Scores:: Scores(const size_t _index) throw() :
        ScoresType(), next(0), prev(0), index(_index),
        absx2(0)
        {
            
        }
        
        void Divide:: Scores:: process()
        {
            assert(size>0);
            merging<Score>:: sort<ScoresType>( *this, Score::CompareByDrecreasingValues, 0);
            size_t   im  = size>>1;
            score_t  mx2 = fetch(im)->items;
            if(0==(size&0x1))
            {
                mx2 += fetch(--im)->items;
            }
            else
            {
                mx2 += mx2;
            }
            score_t &a = (score_t &)absx2;
            a = 0;
            for(const Score *s  = head;s;s=s->next)
            {
                const score_t it = s->items;
                a += abs_of( (it+it) - mx2 );
            }
            while(size>1)
            {
                delete pop_back();
            }
        }
        
        
        std::ostream & operator<<( std::ostream &os, const Divide::Scores &S )
        {
            os << '@' << S.index << ':' << '{';
            for(const Divide::Score *s=S.head;s;s=s->next)
            {
                os << ' ' << *s;
            }
            os << '}';
            return os;
        }
        
        int Divide::Scores:: CompareByAX2AndIncreasingScore(const Scores *lhs, const Scores *rhs, void *) throw()
        {
            if(lhs->absx2<rhs->absx2)
            {
                return -1;
            }
            else if( rhs->absx2<lhs->absx2 )
            {
                return 1;
            }
            else
            {
                return - Score::CompareByDrecreasingValues(lhs->head, rhs->head, 0);
            }
                
        }
        
    }
    
}

namespace upsylon
{
    namespace Oxide
    {

        Divide:: Billboard:: Billboard() throw()
        {
        }
        
        Divide:: Billboard:: ~Billboard() throw()
        {
        }
        
        size_t Divide:: Billboard:: getOptimalIndex()
        {
            merging<Scores>:: sort<BillboardType>( *this, Scores::CompareByAX2AndIncreasingScore, 0);
            return head->index;
        }
        
    }
    
}

