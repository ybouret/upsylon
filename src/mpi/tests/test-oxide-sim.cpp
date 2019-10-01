#include "y/oxide/mpi/district.hpp"
#include "y/oxide/field3d.hpp"
#include "y/oxide/vtk.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{

    template <typename T>
    void fill( FieldOf<T> &F )
    {
        for(size_t i=F.localObjects/2;i>0;--i)
        {
            const T tmp = support::get<T>();
            F.entry[ alea.range<Coord1D>(0,F.localObjects-1) ] = tmp;
        }
    }

    template <typename COORD>
    void make_for(mpi                 &MPI,
                  const Layout<COORD> &fullLayout )
    {
        typedef typename FieldFor<COORD,double>::Type   dField;
        typedef typename FieldFor<COORD,Coord1D>::Type  iField;

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
            iField         &Fi = W.template createExported<iField>("Fi");
            dField         &Fd = W.template createExported<dField>("Fd");
            
            fill(Fd);
            Fi.ld(MPI.rank);

            if(W.realm)
            {
                fill( W.realm->template as<dField>("Fd") );
            }


            W.Gather("Fi");
            if(W.realm&&pbc.index<=1)
            {
                const string  filename = vformat("realm%d.vtk", int(W.Dimensions));
                ios::ocstream fp(filename);

                const vtk &VTK = vtk::instance();
                const Layout<COORD> &L = W.realm->inner;
                VTK.writeHeader(fp);
                VTK.writeTitle(fp,filename);
                VTK.writeLayout(fp,L);
                VTK.writePointData(fp,L);
                VTK.writeField(fp,W.realm->template as<iField>("Fi"), L);
            }

            W.Scatter("Fd");

            ActiveFields fields;
            fields(W);

            W.localExchange(fields);
            W.asyncExchange(fields);

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
        const Layout1D full1D( lower.x,    upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        make_for(MPI,full1D);
        make_for(MPI,full2D);
        make_for(MPI,full3D);

    }

}
Y_UTEST_DONE()


