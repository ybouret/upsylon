//! \file
#ifndef Y_OXIDE_OPTIMAL_LAYOUT_INCLUDED
#define Y_OXIDE_OPTIMAL_LAYOUT_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace Oxide
    {

        struct OptimalLayout
        {

            template <typename COORD>
            class Score
            {
            public:
                const COORD  mapping;
                const size_t maxItems;

                inline Score(const COORD & userMapping, const size_t userMaxItems ) throw() :
                mapping(  userMapping  ),
                maxItems( userMaxItems )
                {
                }

                inline Score( const Score &_ ) throw() :
                mapping(  _.mapping  ),
                maxItems( _.maxItems )
                {
                }


                inline ~Score() throw()
                {

                }

                inline friend std::ostream & operator<<( std::ostream &os, const Score &s )
                {
                    os << '(' << s.maxItems << '@' << s.mapping << ')';
                    return os;
                }


            private:
                Y_DISABLE_ASSIGN(Score);
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

            template <typename COORD> static inline
            void Find( const Layout<COORD> &full, const size_t cores )
            {
                assert(cores>0);
                typedef Score<COORD> ScoreType;
                vector<ScoreType> scores;
                Context<COORD>    context = { &full, &scores };
                full.forEachMapping(cores,AnalyzeMapping,&context);
                std::cerr << "scores: " << scores << std::endl;
            }

        private:
            template <typename COORD>
            struct Context
            {
                const Layout<COORD>      *full;
                sequence< Score<COORD> > *scores;
            };

        };

    }
}

#endif

