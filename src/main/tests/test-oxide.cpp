#include "y/oxide/layout.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{

    template <typename LAYOUT>
    static inline void testLayout()
    {
        typedef typename LAYOUT::coord coord;
        std::cerr << "-- Layout Dimensions=" << LAYOUT::Dimensions << std::endl;
        for(size_t cycle=1;cycle<=32;++cycle)
        {
            const LAYOUT L( Oxide::Coord::Integer<coord>(30,alea), Oxide::Coord::Integer<coord>(30,alea) );
            std::cerr << "\tL=" << L << std::endl;
            std::cerr << "\ttesting indices" << std::endl;

            for(size_t i=L.items*8;i>0;--i)
            {
                const coord          C = L.rand( alea );
                const Oxide::Coord1D I = L.indexOf(C);
                Y_ASSERT(I>=0);
                Y_ASSERT(I<Oxide::Coord1D(L.items));
            }

            std::cerr << "\ttesting conversion" << std::endl;
            for(size_t idx=0;idx<L.items;++idx)
            {
                const coord  C   = L.coordOf(idx);
                const size_t jdx = L.indexOf(C);
                Y_ASSERT(idx==jdx);
            }

            std::cerr << "\ttesting Loop" << std::endl;
            vector<Oxide::Coord1D> iv;
            list<Oxide::Coord1D>   il;
            {
                typename LAYOUT::Loop mloop(L.lower,L.upper);

                size_t nn = 0;
                for( mloop.start(); mloop.valid(); mloop.next() )
                {
                    if(mloop.index==1)
                    {
                        std::cerr << "\t\tini=" << mloop.value << std::endl;
                    }
                    if(mloop.index==L.items)
                    {
                        std::cerr << "\t\tend=" << mloop.value << std::endl;
                    }
                    ++nn;
                }
                std::cerr << "\t\tnn=" << nn << "/" << L.items << std::endl;
            }



            for(size_t iter=0;iter<2;++iter)
            {
                const LAYOUT sub( L.rand(alea), L.rand(alea) );
                std::cerr << "\tsub=" << sub << std::endl;
                L.collect(iv,sub);
                L.collect(il,sub);
                Y_ASSERT(iv.size()==il.size());
            }
            std::cerr << "\t\tcollected: " << iv.size() << "/" << il.size() << std::endl;
        }

    }

#define DISPLAY_COORD(FIELD)\
std::cerr << #FIELD << " : " << Oxide::Coord::Get<COORD>::FIELD << std::endl
    template <typename COORD>
    static inline void testCoord()
    {
        DISPLAY_COORD(Dimensions);
        //DISPLAY_COORD(LocalNodes);
        //DISPLAY_COORD(Neighbours);
    }
}


Y_UTEST(oxide)
{
    testLayout<Oxide::Layout1D>();
    testLayout<Oxide::Layout2D>();
    testLayout<Oxide::Layout3D>();

    testCoord<Oxide::Coord1D>();
    testCoord<Oxide::Coord2D>();
    testCoord<Oxide::Coord3D>();


}
Y_UTEST_DONE()
