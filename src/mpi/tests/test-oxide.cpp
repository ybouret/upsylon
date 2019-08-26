
#include "y/oxide/field/mpi.hpp"
#include "y/oxide/field3d.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Oxide;

Y_UTEST(oxide)
{
    Y_MPI(SINGLE);
    
    {
        Coord1D  lo = CoordOps::Integer<Coord1D>(20,alea);
        Coord1D  up = CoordOps::Integer<Coord1D>(20,alea);
        MPI.Bcast(lo,0);
        MPI.Bcast(up,0);
        const Layout1D L(lo,up);
        MPI.print(stderr, "full: lo=%ld, hi=%ld\n", long(L.lower), long(L.upper) );
        lo = L.rand(alea);
        up = L.rand(alea);
        MPI.Bcast(lo,0);
        MPI.Bcast(up,0);
        const Layout1D s(lo,up);
        MPI.print(stderr, "sub:  lo=%ld, hi=%ld\n", long(s.lower), long(s.upper) );

    }
    
    
}
Y_UTEST_DONE()

