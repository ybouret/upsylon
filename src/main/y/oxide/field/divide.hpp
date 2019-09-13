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
            typedef Coord1D      score_t;
            
            class Score : public object
            {
            public:
                Score *next;
                Score *prev;
                //const size_t  index;
                const score_t items;
                const score_t async;
                const score_t local;
                
                virtual ~Score() throw();
                explicit Score(const score_t &_items,
                               const score_t &_async,
                               const score_t &_local);
                
                friend std::ostream & operator<<( std::ostream &os, const Score &s );
                static int CompareByDrecreasingValues(const Score *lhs, const Score *rhs, void *) throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Score);
            };
            
            typedef core::list_of_cpp<Score> ScoresType;
            
            class Scores : public ScoresType, public object
            {
            public:
                Scores       *next;
                Scores       *prev;
                const size_t  index;
                const score_t absx2; //!< absolute deviation x 2
                
                virtual ~Scores() throw();
                explicit Scores(const size_t _index) throw();
                
                template <typename COORD>
                void append( const Layouts<COORD> & layouts )
                {
                    const score_t items( layouts.inner.items );
                    const score_t async( layouts.asyncComms  );
                    const score_t local( layouts.localComms  );
                    push_back( new Score(items,async,local) );
                }
                
                friend std::ostream & operator<<(std::ostream &os, const Scores &S);
                
                void process(); //!< and compute mx2
                
                static int CompareByAX2AndIncreasingScore(const Scores *lhs, const Scores *rhs, void *) throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scores);
            };
            
            typedef core::list_of_cpp<Scores> BillboardType;
            
            class Billboard : public BillboardType
            {
            public:
                explicit Billboard() throw();
                virtual ~Billboard() throw();
                size_t   getOptimalIndex();
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Billboard);
            };
            
            template <typename COORD> static inline
            COORD Find(const Layout<COORD> &full,
                       const Coord1D        cores,
                       const COORD         &pbc,
                       sequence<COORD>     *out=0)
            {
                static const size_t initialCapacity = InitialCapacity[Coord::Get<COORD>::Dimensions];
                
                //--------------------------------------------------------------
                //
                // initialize and find all mappings
                //
                //--------------------------------------------------------------
                if(out)
                {
                    out->free();
                }
                vector<COORD,memory::pooled> mappings(initialCapacity,as_capacity);
                full.buildMappings(mappings,cores);
                
                if(mappings.size()<=0)
                {
                    COORD  z(0);
                    Coord::LD(z,0);
                    return z;
                }
                else
                {
                    //! analyze
                    Billboard billboard;
                    for(size_t m=1;m<=mappings.size();++m)
                    {
                        Scores        *scores   = billboard.push_back( new Scores(m) );
                        const COORD    &mapping = mappings[m];
                        //std::cerr << " |_mapping=" << mapping << ":";
                        for(Coord1D rank=0;rank<cores;++rank)
                        {
                            const Layouts<COORD>  layouts(full,mapping,rank,pbc,1);
                            scores->append( layouts );
                        }
                        scores->process();
                        //std::cerr << *scores << " @ absx2=" << scores->absx2 << std::endl;
                    }
                    const size_t optimalIndex = billboard.getOptimalIndex();
                    for(const Scores *scores=billboard.head;scores;scores=scores->next)
                    {
                        std::cerr << " |_mapping: " << mappings[scores->index] << " : " << *scores << ", absx2=" << scores->absx2 << std::endl;
                    }
                    
                    //std::cerr << " |_optimal=" << mappings[optimalIndex] << ':' << *(billboard.head) << std::endl;
                    if(out)
                    {
                        out->ensure(billboard.size);
                        for(const Scores *scores=billboard.head;scores;scores=scores->next)
                        {
                            out->push_back( mappings[scores->index] );
                        }
                    }
                    return mappings[optimalIndex];
                }
            }
            
        };
    }
}

#endif

