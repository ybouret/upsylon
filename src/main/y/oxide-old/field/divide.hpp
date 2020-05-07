// !\file
#ifndef Y_OXIDE_FIELD_DIVIDE_INCLUDED
#define Y_OXIDE_FIELD_DIVIDE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! field division algorithm
        struct Divide
        {
            static  const size_t InitialCapacity[4]; //!< maximal number of mappings (approx)

            //! objective score structure
            class Score : public object
            {
            public:
                static const unsigned ABSX2 = 0;                         //!< twice the absolute deviation of the sub-layouts items
                static const unsigned ITEMS = 1;                         //!< index for sub-layout items
                static const unsigned ASYNC = 2;                         //!< index for sub-layout asyncComms
                static const unsigned LOCAL = 3;                         //!< index for sub-layout localComms
                static const unsigned DIM0  = 4;                         //!< index for sub-layout width[0]
                static const unsigned DIM1  = 5;                         //!< index for sub-layout width[1]
                static const unsigned DIM2  = 6;                         //!< index for sub-layout width[2]
                static const unsigned MAX_LENGTH = DIM2+1;               //!< max lenght, in 3D
                static const unsigned DATA_COUNT = Y_ROUND2(MAX_LENGTH); //!< rounded
                static const size_t   LENGTH[4];                         //!< full score length according to Dimensions
                
                Score       *next;             //!< for list
                Score       *prev;             //!< for list
                const size_t dims;             //!< 1/2/3
                const size_t indx;             //!< global rank to retrieve mapping
                Coord1D      data[DATA_COUNT]; //!< all the data builind the socre

                //! setup with dimensions(=dims) and mappingIndex(=indx)
                explicit Score(const size_t dimensions,
                               const size_t mappingIndex) throw();
                //! cleanup
                virtual ~Score() throw();

                //! display, mostly to debug
                friend std::ostream & operator<<( std::ostream &, const Score &);

                //! lexicographic decrease( &data[ITEMS], 3 )
                static int CompareByDecreasingComms( const Score *lhs, const Score *rhs, void *) throw();

                //! lexicographics increase( &data[ABSX2], LENGTH[dims] )
                static int FullyCompare( const Score *lhs, const Score *rhs, void *) throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Score);
            };

            //! base class for list of Scores
            typedef core::list_of_cpp<Score> ScoresType;

            //! a list of scores
            class Scores : public ScoresType
            {
            public:
                explicit Scores() throw(); //!< setup
                virtual ~Scores() throw(); //!< cleanup

                //! convert a mapping into a score
                template <typename COORD>
                void append(const COORD          &mapping,
                            const size_t          mappingIndex,
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


                //! display
                friend std::ostream & operator<<( std::ostream &, const Scores &);

                //! select the worst item after absx2 computation
                void   selectFromMapping();

                //! select the best index after full comparison
                size_t getOptimalIndex();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scores);
            };
            
            //! find the optimal mapping according to compute and I/O data
            /**
             if out is not NULL, then the different mappings are stored, from the
             most to the least efficient
             */
            template <typename COORD> static inline
            COORD Find(const Layout<COORD> &fullLayout,
                       const size_t         cores,
                       const COORD         &boundaries,
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
                fullLayout.buildMappings(mappings,cores);
                const size_t numMappings = mappings.size();

                if(numMappings<=0)
                {
                    return Coord::Zero<COORD>();
                }
                else
                {
                    //----------------------------------------------------------
                    // build a grade per mapping
                    //----------------------------------------------------------
                    Scores grades;
                    for(size_t mappingIndex=1;mappingIndex<=numMappings;++mappingIndex)
                    {
                        // collect all scored
                        const COORD         &mapping = mappings[mappingIndex];
                        Scores               scores;
                        for(size_t rank=0;rank<cores;++rank)
                        {
                            const Layouts<COORD> layouts(fullLayout,mapping,rank,boundaries,1);
                            scores.append(mapping,mappingIndex,layouts);
                        }
                        // get the score of thi mapping
                        scores.selectFromMapping();

                        // and keep it for global comparison
                        grades.push_back(scores.pop_front());
                    }

                    //----------------------------------------------------------
                    // get the optimal index
                    //----------------------------------------------------------
                    const size_t opt = grades.getOptimalIndex();
                    if(out)
                    {
                        assert(0==out->size());
                        out->ensure(grades.size);
                        for(const Score *score=grades.head;score;score=score->next)
                        {
                            out->push_back( mappings[score->indx] );
                        }
                    }
                    return mappings[opt];
                }
            }

            //! throw exception if no possible mapping
            static void ErrorNoMappingFor(const size_t cores);
            
        };
    }
}

#endif

