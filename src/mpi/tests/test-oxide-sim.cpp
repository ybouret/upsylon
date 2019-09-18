#include "y/oxide/mpi/district.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    template <typename COORD>
    void make_for(mpi                 &MPI,
                  const Layout<COORD> &fullLayout )
    {

        if(MPI.isHead)
        {
            MPI.flush(stderr);
            std::cerr << "Using " << fullLayout << std::endl;
            std::cerr.flush();
        }

        const size_t ghostsZone = 1;
        const COORD  preferred  = Coord::Ones<COORD>();



        typename Layout<COORD>::Loop pbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());
        MPI.print0(stderr,"[");
        for(pbc.start(); pbc.valid(); pbc.next())
        {
            MPI.print0(stderr,".");
            const COORD boundaries = pbc.value;
            District<COORD> W(MPI,fullLayout,boundaries,ghostsZone,preferred);
            
        }
        MPI.print0(stderr,"]\n");
        
    }

}

Y_UTEST(oxide_sim)
{
    Y_MPI(SINGLE);

    const Coord3D  lower(1,1,1);
    const Coord3D  org(1,1,1);
    const Coord3D  top(2,2,2);
    Layout3D::Loop loop(org,top);

    for( loop.start(); loop.valid(); loop.next() )
    {
        const Coord3D  upper = 8 * loop.value;
        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        make_for(MPI,full1D);
        make_for(MPI,full2D);
        make_for(MPI,full3D);

    }

}
Y_UTEST_DONE()


