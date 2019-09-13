#include "y/oxide/field3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/oxide/algo/optimal.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/layouts.hpp"

#include "support.hpp"
#include <typeinfo>
#include "y/string/tokenizer.hpp"
#include "y/sort/merge.hpp"
#include "y/mpl/rational.hpp"

using namespace upsylon;
using namespace Oxide;


namespace
{
    typedef vector<string> strings;
    
    template <typename COORD>
    static inline void split_layout( const Layout<COORD> &L, const size_t nmax)
    {
        std::cerr << "In " << L.Dimensions << "D" << std::endl;
        std::cerr << "L=" << L << std::endl;
        vector<COORD> mappings(nmax,as_capacity);

        for(size_t cores=nmax;cores<=nmax;++cores)
        {
            // let's take a given #cores
            // and build the possible mappings for this layoyt
            std::cerr << "\tcores=" << cores << std::endl;
            L.buildMappings(mappings,cores);
            std::cerr << "\tmappings=" << mappings << std::endl;
            if(mappings.size()<=0)
            {
                std::cerr << "\t\tfailure" << std::endl;
                continue;
            }

            // for each valid mapping, compute associated partition
            const size_t npart = mappings.size();
            std::cerr << "\ttesting partitions: " << std::endl;
            vector< Layout<COORD> > partition;
            for(size_t i=1;i<=npart;++i)
            {
                const COORD &mapping = mappings[i];
                std::cerr << "\t\t" << mapping << " [[";
                L.buildPartition(partition,mapping);
                Y_ASSERT(cores==partition.size());
                for(size_t j=1;j<=partition.size();++j)
                {
                    std::cerr << " " << partition[j];
                }
                std::cerr << " ]] maxItems=" << L.getPartitionMaxItems(mapping) << std::endl;

            }
#if 0
            std::cerr << "Using Optimal API" << std::endl;
            {
                vector< Optimal::Score<COORD> > scores;
                Optimal::Find(scores,L,cores);
                std::cerr << "scores=" << scores << std::endl;
            }

            const COORD opt = Optimal::Find(L,cores);
            std::cerr << "Found to be " << opt << std::endl;
#endif
        }
    }

    class Score : public object
    {
    public:
        Score    *next;
        Score    *prev;
        const mpq items;
        const mpq async;
        const mpq local;

        explicit Score(const size_t _items,
                       const size_t _async,
                       const size_t _local
                       ) :
        next(0), prev(0),
        items(_items),
        async(_async),
        local(_local)
        {

        }

        virtual ~Score() throw()
        {

        }

        static inline int CompareDecreasing(const Score *lhs, const Score *rhs, void *) throw()
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

        typedef core::list_of_cpp<Score> ListType;

        class List : public ListType, public object
        {
        public:

            List        *next;
            List        *prev;
            const size_t indx;  //!< mapping index
            mpq          alpha; //!< worst case scenario

            explicit List(const size_t i) throw() :
            next(0), prev(0), indx(i), alpha(0)
            {
                assert(i>0);
            }

            virtual ~List() throw()
            {
            }

            void sort()
            {
                merging<Score>::sort<ListType>(*this,Score::CompareDecreasing,0);
            }

            // keep only the highest async for same items
            void shrink1() throw()
            {
                assert(size>0);
                ListType tmp;
                tmp.push_back( pop_front() );
                while(size)
                {
                    Score *score = pop_front();
                    if( (score->items>=tmp.tail->items) )
                    {
                        delete score;
                    }
                    else
                    {
                        tmp.push_back(score);
                    }
                }
                swap_with(tmp);
            }

            void shrink2() throw()
            {
                assert(size>0);
                ListType tmp;
                tmp.push_back( pop_front() );
                while(size)
                {
                    Score *score = pop_front();
                    assert(score->items<tmp.tail->items);
                    if( (score->async<=tmp.tail->async) )
                    {
                        delete score;
                    }
                    else
                    {
                        tmp.push_back(score);
                    }
                }
                swap_with(tmp);

            }

            void computeAlpha()
            {
                assert(0==alpha);
                for(const Score *score=head->next;score;score=score->next)
                {
                    assert(score->items<head->items);
                    assert(score->async>head->async);
                    const mpq tmp = (head->items-score->items)/(score->async-head->async);
                    if(tmp>alpha) alpha = tmp;
                }
            }

            void process()
            {
                sort();
                shrink1();
                shrink2();
                computeAlpha();
            }


            friend std::ostream & operator<<( std::ostream &os, const List &scores )
            {
                os << '{';
                for(const Score *score = scores.head; score; score=score->next)
                {
                    os << ' ' << '(' << score->items << ',' << score->async << ',' << score->local << ')';
                }
                os << '}' << '/' << scores.alpha;
                return os;
            }

            static inline int CompareByIncreasingFirstScore( const List *lhs, const List *rhs, void *) throw()
            {
                assert(lhs->size>0);
                assert(rhs->size>0);
                return -Score::CompareDecreasing(lhs->head,rhs->head,0);
            }

            typedef core::list_of_cpp<List> CatalogType;

            class Catalog : public CatalogType
            {
            public:
                explicit Catalog() throw() : CatalogType()
                {
                }

                virtual ~Catalog() throw()
                {
                }



                void cleanup() throw()
                {
                    assert(size>0);
                    merging<List>::sort<CatalogType>( *this, List::CompareByIncreasingFirstScore, 0);

                    CatalogType tmp;
                    tmp.push_back( pop_front() );
                    while(size)
                    {
                        List        *scores  = pop_front(); assert(scores->size>0);
                        const Score *curr    = scores->tail;
                        const Score *last    = tmp.tail->tail;
                        if( ( (curr->items>last->items) && (curr->async>=last->async) )
                           ||
                           ( ( curr->items>=last->items) && (curr->async>last->async) )
                           )
                        {
                            delete scores;
                        }
                        else
                        {
                            tmp.push_back( scores );
                        }
                    }
                    swap_with(tmp);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Catalog);
            };



        private:
            Y_DISABLE_COPY_AND_ASSIGN(List);
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Score);
    };

    template <typename COORD>
    void doSplit( const char *args , const Coord1D cores )
    {
        COORD       ini(0); Coord::LD(ini,0);
        COORD       org(1); Coord::LD(org,1);
        const COORD top   = Coord::Parse<COORD>(args,"length");

        typename Layout<COORD>::Loop pbc(ini,org);
        const Layout<COORD>          full(org,top);
        std::cerr << "full=" << full << std::endl;

        vector<COORD> mappings;
        full.buildMappings(mappings,cores);
        std::cerr << "mappings: " << mappings << std::endl;
        if( mappings.size() )
        {
            for(pbc.start(); pbc.valid(); pbc.next() )
            {
                std::cerr << "|_pbc=" << pbc.value << std::endl;
                Score::List::Catalog catalog;
                for(size_t m=1;m<=mappings.size();++m)
                {
                    const COORD &mapping = mappings[m];
                    std::cerr << " |_using: " << mapping << ":";
                    Score::List *scores = catalog.push_back( new Score::List(m) );
                    for(Coord1D rank=0;rank<cores;++rank)
                    {
                        const Layouts<COORD> sub( full, mapping, rank, pbc.value, 1 );
                        const size_t items = sub.inner.items;
                        const size_t local = sub.localComms;
                        const size_t async = sub.asyncComms;
                        scores->push_back( new Score(items, async, local) );
                    }
                    scores->process();
                    std::cerr << " => " << *scores << std::endl;
                }

                catalog.cleanup();
                for(Score::List *scores = catalog.head; scores; scores=scores->next)
                {
                    std::cerr << " |_@" << mappings[scores->indx] << " : " << *scores << std::endl;
                }



            }

        }

    }

}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(oxide_split)
{
    // length pbc ng mapping
    if(argc<=2)
    {
        std::cerr << "Usage: " << program << " length cores" << std::endl;
        return 0;
    }


    strings       length(3,as_capacity);
    const size_t  dimensions = tokenizer<char>::split_with(length, argv[1], ',');
    const Coord1D cores      = string_convert::to<size_t>(argv[2],"cores"); if(cores<=0) throw exception("cores<=0");


    switch( dimensions )
    {
        case 1: doSplit<Coord1D>( argv[1], cores );break;
        case 2: doSplit<Coord2D>( argv[1], cores );break;
        case 3: doSplit<Coord3D>( argv[1], cores );break;
        default:
            throw exception("Invalid dimensions");
    }

#if 0
    size_t nmax=2; if(argc>1) nmax = string_convert::to<size_t>(argv[1],"nmax");
    
    const Coord3D org(1,1,1);
    Coord3D       top(10,20,30);
    if(argc>2)    top = Coord::Parse<Coord3D>(argv[2]);


    {
        Layout1D L(org.x,top.x);
        split_layout(L,nmax);;
    }

    {
        Layout2D L(org.xy(),top.xy());
        split_layout(L,nmax);
    }

    {
        Layout3D L(org,top);
        split_layout(L,nmax);
    }
#endif





}
Y_UTEST_DONE()

namespace
{
    template <typename COORD> static inline
    size_t maxSplits( const size_t cores )
    {

        size_t count = 0;

        if(cores>0)
        {
            COORD  org(0);
            COORD  top(0);
            Coord::LD(org,1);
            Coord::LD(top,cores);
            mloop<Coord1D,COORD> loop( org, top );

            for( loop.start(); loop.valid(); loop.next() )
            {
                if( cores == size_t( Coord::Product(loop.value) ) )
                {
                    ++count;
                }
            }

        }
        return count;
    }
}

Y_UTEST(oxide_max_splits)
{
    std::cerr << "Computing MaxSplits" << std::endl;
    ios::ocstream fp("splits.dat");
    for(unsigned long cores=0;cores<=128;++cores)
    {
        const unsigned long m1 = maxSplits<Coord1D>(cores);
        const unsigned long m2 = maxSplits<Coord2D>(cores);
        const unsigned long m3 = maxSplits<Coord3D>(cores);
        fp("%lu %lu %lu %lu\n",cores,m1,m2,m3);

    }
}
Y_UTEST_DONE()
