//! \file
#ifndef Y_OXIDE_OPTIMAL_LAYOUT_INCLUDED
#define Y_OXIDE_OPTIMAL_LAYOUT_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/sort/heap.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! finding an optimal mapping
        struct OptimalLayout
        {

            template <typename COORD>
            class Score
            {
            public:
                const COORD   mapping;
                const Coord1D penality;
                const size_t  maxItems;

                inline Score(const COORD & userMapping, const size_t userMaxItems ) throw() :
                mapping(  userMapping  ),
                penality( Coord::Norm1(mapping) ),
                maxItems( userMaxItems )
                {
                }

                inline Score( const Score &_ ) throw() :
                mapping(  _.mapping  ),
                penality( _.penality ),
                maxItems( _.maxItems )
                {
                }


                inline ~Score() throw()
                {

                }

                inline friend std::ostream & operator<<( std::ostream &os, const Score &s )
                {
                    os << '(' << s.maxItems << '@' << '|' << s.mapping << '|' << '=' << s.penality << ')';
                    return os;
                }

                static inline int CompareByMaxItems(const Score &lhs, const Score &rhs) throw()
                {
                    return comparison::increasing(lhs.maxItems,rhs.maxItems);
                }

                static inline int CompareByPenality(const Score &lhs, const Score &rhs) throw()
                {
                    return comparison::increasing(lhs.penality,rhs.penality);
                }

                static inline int CompareByMapping(const Score &lhs, const Score &rhs) throw()
                {
                    return Coord::Compare(lhs.mapping,rhs.mapping);
                }


            private:
                Y_DISABLE_ASSIGN(Score);
            };



            template <typename COORD> static inline
            COORD Find( const Layout<COORD> &full, const size_t cores )
            {
                static const size_t initialCap[4] = { 0, 1, 16, 64 };

                assert(cores>0);

                //______________________________________________________________
                //
                // create score for each partition
                //______________________________________________________________
                typedef Score<COORD> ScoreType;
                vector<ScoreType>    scores(initialCap[Coord::Get<COORD>::Dimensions],as_capacity);
                Context<COORD>       context = { &full, &scores };
                full.forEachMapping(cores,AnalyzeMapping,&context);
                if(scores.size()<=0)
                {
                    return COORD(0);
                }

                //______________________________________________________________
                //
                // keep best max items only
                //______________________________________________________________
                hsort(scores, ScoreType::CompareByMaxItems);
                {
                    const size_t bestMaxItems = scores.front().maxItems;
                    while(scores.size()>1 && scores.back().maxItems>bestMaxItems)
                    {
                        scores.pop_back();
                    }
                }

                //______________________________________________________________
                //
                // keep best penality only
                //______________________________________________________________
                hsort(scores,ScoreType::CompareByPenality);
                {
                    const Coord1D bestPenality = scores.front().penality;
                    while(scores.size()>1 && scores.back().penality>bestPenality)
                    {
                        scores.pop_back();
                    }
                }
                //______________________________________________________________
                //
                // order by preferred dimension
                //______________________________________________________________
                hsort(scores,ScoreType::CompareByMapping);
                while(scores.size()>1) scores.pop_back();
                return scores.front().mapping;
            }

        private:
            template <typename COORD> struct Context
            {
                const Layout<COORD>      *full;
                sequence< Score<COORD> > *scores;
            };

            template <typename COORD>
            static inline void AnalyzeMapping( const COORD &mapping, void *args )
            {
                assert(args);
                Context<COORD>    &context  = *static_cast< Context<COORD> * >(args);
                const size_t       maxItems = context.full->getPartitionMaxItems(mapping);
                const Score<COORD> score(mapping,maxItems);
                context.scores->push_back(score);
            }
        };

    }
}

#endif

