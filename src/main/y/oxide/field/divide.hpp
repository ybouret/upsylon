// !\file
#ifndef Y_OXIDE_FIELD_DIVIDE_INCLUDED
#define Y_OXIDE_FIELD_DIVIDE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace Oxide
    {
        struct Divide
        {
            static  const size_t InitialCapacity[4];
            
            class Score : public object
            {
            public:
                static const unsigned ABSX2 = 0;
                static const unsigned ITEMS = 1;
                static const unsigned ASYNC = 2;
                static const unsigned LOCAL = 3;
                static const unsigned DIM0  = 4;
                static const unsigned DIM1  = 5;
                static const unsigned DIM2  = 6;
                static const unsigned MAX_LENGTH = DIM2+1;
                static const unsigned DATA_COUNT = Y_ROUND2(MAX_LENGTH);
                static const size_t   LENGTH[4];
                
                Score       *next;
                Score       *prev;
                const size_t dims;
                const size_t indx;
                Coord1D      data[DATA_COUNT];
                
                explicit Score(const size_t dimensions,
                               const size_t mappingIndex) throw();
                virtual ~Score() throw();
                
                friend std::ostream & operator<<( std::ostream &, const Score &);
                
                static int CompareByDecreasingComms( const Score *lhs, const Score *rhs, void *) throw();
                static int FullyCompare( const Score *lhs, const Score *rhs, void *) throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Score);
            };
            
            typedef core::list_of_cpp<Score> ScoresType;
            
            class Scores : public ScoresType
            {
            public:
                explicit Scores() throw();
                virtual ~Scores() throw();
                
                template <typename COORD>
                void append(const COORD &mapping,
                            const size_t mappingIndex,
                            const Layouts<COORD> &layouts )
                {
                    // keep local dimensions
                    static const size_t DIMS = Coord::Get<COORD>::Dimensions;
                    
                    // create a new score
                    Score *score = push_back( new Score(DIMS,mappingIndex) );
                    
                    // fill the comms part
                    score->data[ Score::ITEMS ] = layouts.inner.items;
                    score->data[ Score::ASYNC ] = layouts.asyncComms;
                    score->data[ Score::LOCAL ] = layouts.localComms;
                    
                    // fill the mapping part
                    for(size_t dim=0;dim<DIMS;++dim)
                    {
                        score->data[ Score::DIM0 + dim ] = Coord::Of(mapping,dim);
                    }
                }
                
                friend std::ostream & operator<<( std::ostream &, const Scores &);
                void   selectFromMapping();
                size_t getOptimalIndex();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scores);
            };
            
            
            
            template <typename COORD> static inline
            COORD Find(const Layout<COORD> &full,
                       const size_t         cores,
                       const COORD         &pbc,
                       sequence<COORD>     *out=0)
            {
                static const size_t initialCapacity = InitialCapacity[Coord::Get<COORD>::Dimensions];
                
                //--------------------------------------------------------------
                //
                // initialize and find all mappings
                //
                //--------------------------------------------------------------
                if(out) out->free();
                vector<COORD,memory::pooled> mappings(initialCapacity,as_capacity);
                full.buildMappings(mappings,cores);
                const size_t numMappings = mappings.size();

                if(numMappings<=0)
                {
                    COORD  z(0);
                    Coord::LD(z,0);
                    return z;
                }
                else
                {
                    Scores grades;
                    for(size_t mappingIndex=1;mappingIndex<=numMappings;++mappingIndex)
                    {
                        const COORD         &mapping = mappings[mappingIndex];
                        Scores               scores;
                        for(size_t rank=0;rank<cores;++rank)
                        {
                            const Layouts<COORD> layouts(full,mapping,rank,pbc,1);
                            scores.append(mapping,mappingIndex,layouts);
                        }
                        scores.selectFromMapping();
                        grades.push_back(scores.pop_front());
                    }
                    const size_t opt = grades.getOptimalIndex();
                    std::cerr << "|_grades=" << grades << std::endl;
                    if(out)
                    {
                        for(const Score *score=grades.head;score;score=score->next)
                        {
                            out->push_back( mappings[score->indx] );
                        }
                    }
                    return mappings[opt];
                }
            }
            
        };
    }
}

#endif

