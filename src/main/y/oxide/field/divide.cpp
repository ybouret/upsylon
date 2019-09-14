
#include "y/oxide/field/divide.hpp"

namespace upsylon
{
    namespace Oxide
    {
        const size_t Divide:: InitialCapacity[4] = { 0, 1, 16, 64 };
        
        const size_t Divide:: Score:: LENGTH[4] =
        {
            0, DIM0+1, DIM1+1, DIM2+1
        };
        
        
        Divide:: Score:: Score(const size_t dimensions,
                               const size_t mappingIndex) throw() :
        next(0),
        prev(0),
        dims( dimensions   ),
        indx( mappingIndex ),
        data()
        {
            assert(dims>=1); assert(dims<=3);
            memset(data,0,sizeof(data));
        }
        
        Divide:: Score:: ~Score() throw()
        {
        }
        
        int Divide::Score:: CompareByDecreasingComms( const Score *lhs, const Score *rhs, void *) throw()
        {
            assert(lhs); assert(rhs);
            return comparison::decreasing_lexicographic(lhs->data+Score::ASYNC,
                                                        rhs->data+Score::ASYNC,
                                                        3);
        }
        
        int Divide::Score:: FullyCompare( const Score *lhs, const Score *rhs, void *) throw()
        {
            assert(lhs); assert(rhs);
            assert(lhs->dims==rhs->dims);
            assert(lhs->dims>=1);
            assert(lhs->dims<=3);

            return comparison::increasing_lexicographic(lhs->data,
                                                        rhs->data,
                                                        LENGTH[lhs->dims]);
        }
        
        std::ostream & operator<<( std::ostream &os, const Divide::Score &s )
        {
            os << '@' << s.indx << '<';
            os << '$' << s.data[s.ABSX2];
            os << '(' << s.data[s.ITEMS] << ',' <<  s.data[s.ASYNC] << ',' << s.data[s.LOCAL] << ')';
            os << '[' << s.data[s.DIM0];
            for(size_t i=1;i<s.dims;++i)
            {
                os << ',' << s.data[s.DIM0+i];
            }
            os << ']';
            os << '>';
            
            return os;
        }
        
        Divide:: Scores:: Scores() throw() : ScoresType() {}
        
        Divide:: Scores:: ~Scores() throw()  {}
        
        
        std::ostream & operator<<( std::ostream &os, const Divide::Scores &S)
        {
            os << '{';
            for(const Divide::Score *score = S.head; score; score=score->next)
            {
                os << ' ' << *score;
            }
            os << ' ' << '}';
            return os;
        }

        void Divide:: Scores:: selectFromMapping()
        {
            merging<Score>:: sort<ScoresType>( *this, Score::CompareByDecreasingComms, 0);
            
            // find the median
            size_t  im    = size>>1;
            Coord1D medx2 = fetch(im)->data[ Score::ITEMS ];
            if( 0 == (size&0x01) )
            {
                medx2 += fetch(--im)->data[ Score::ITEMS ];
            }
            else
            {
                medx2 += medx2;
            }

            // find the absolute deviation
            Coord1D absx2 = 0;
            for(const Score *score = head; score; score=score->next )
            {
                const Coord1D items = score->data[ Score::ITEMS ];
                absx2 += abs_of( (items+items) - medx2 );
            }
           // std::cerr << "absx2=" << absx2 << std::endl;
            
            head->data[ Score::ABSX2 ] = absx2;
            while(size>1)
            {
                delete pop_back();
            }
        }

        size_t Divide:: Scores:: getOptimalIndex()
        {
            assert(size>0);
            merging<Score>:: sort<ScoresType>( *this, Score::FullyCompare, 0);
            return head->indx;
        }

        
    }
}
