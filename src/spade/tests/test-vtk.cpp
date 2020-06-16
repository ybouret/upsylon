#include "y/spade/vtk.hpp"
#include "y/spade/vertices.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Spade;

namespace {

    template <typename T>
    void doVTK()
    {
        vtk &VTK = vtk::instance();
        ios::ocstream fp(ios::cstderr);

        std::cerr << type_name_of<T>() << std::endl;
        const vtk::Writer &w = VTK.getWriter<T>();
        std::cerr << "|_type: '" << w.dataType() << "'" << std::endl;
        std::cerr << "|_comp: " << w.components() << std::endl;

        for(size_t i=5+alea.leq(30);i>0;--i)
        {
            const T tmp = support::get<T>();
            VTK(fp << '\t', tmp) << '\n';
        }

        std::cerr << std::endl;
    }

    template <typename COORD> static inline
    void doSave()
    {
        Coord::DispWidth = 2;
        static const Coord1D r[] = { 10, 20 , 30 };

        
        vtk &VTK = vtk::instance();

        const unsigned dims = Coord::Get<COORD>::Dimensions;
        std::cerr << "VTK" << dims << "D" << std::endl;

        const string   fn   = vformat("in%ud.vtk",dims);
        ios::ocstream  fp(fn);
       
        const string   rn  = vformat("r%ud.vtk",dims);
        ios::ocstream  rp(rn);

        const string   cn  = vformat("c%ud.vtk",dims);
        ios::ocstream  cp(cn);


        VTK.writeHeader(fp);
        VTK.writeTitle(fp,fn);

        VTK.writeHeader(rp);
        VTK.writeTitle(rp,rn);

        VTK.writeHeader(cp);
        VTK.writeTitle(cp,cn);
        
        const Layout<COORD> L( Coord::Ones<COORD>(), *(COORD *) &r[0] );
        
        std::cerr << "Layout: " << L << std::endl;
        VTK.writeLayout(fp,L);
        
        RectilinearMesh<COORD,float> rmesh("rmesh",L);
        typedef typename  RectilinearMesh<COORD,float>::Vertex rvtx;
        typedef typename  RectilinearMesh<COORD,float>::Box    rbox;

        {
            static const float   vlo[] = { -1, -1, -1 };
            static const float   vup[] = {  1,  1,  1 };
            rbox box( *(rvtx *) &vlo[0], *(rvtx *)&vup );
            std::cerr << "rbox=" << box << std::endl;
            rmesh.mapRegular(box,rmesh);
        }
        VTK.writeMesh(rp,rmesh);

        CurvilinearMesh<COORD,double> cmesh("cmesh",L);
        typedef typename  CurvilinearMesh<COORD,double>::Vertex cvtx;
        typedef typename  CurvilinearMesh<COORD,double>::Box    cbox;

        {
            static const double   vlo[] = { -1, -1, -1 };
            static const double   vup[] = {  1,  1,  1 };
            cbox box( *(cvtx *) &vlo[0], *(cvtx *)&vup );
            std::cerr << "cbox=" << box << std::endl;
            cmesh.mapRegular(box,cmesh);
        }
        VTK.writeMesh(cp,cmesh);


        VTK.writePointData(fp,L);
        VTK.writePointData(rp,L);
        VTK.writePointData(cp,L);

        typedef typename FieldFor<COORD>:: template Of<int>::    Type iField;
        typedef typename FieldFor<COORD>:: template Of<float>::  Type fField;
        typedef typename FieldFor<COORD>:: template Of<double>:: Type dField;

        typedef typename VertexFor<COORD>:: template Of<float>::  Type vf_t;
        typedef typename VertexFor<COORD>:: template Of<double>:: Type vd_t;

        typedef typename FieldFor<COORD>:: template Of<vf_t>:: Type vField;
        typedef typename FieldFor<COORD>:: template Of<vd_t>:: Type uField;



        iField I("I",L);
        {
            typename iField::Loop loop(I.lower,I.upper);
            for(loop.boot();loop.good();loop.next())
            {
                I[ *loop ] = int( loop.index );
                if(2==dims)
                {
                   // std::cerr << "@" << *loop << " : " << I[*loop] << std::endl;
                }
            }
        }

        fField F("F",L);
        {
            typename fField::Loop loop(F.lower,F.upper);
            for(loop.boot();loop.good();loop.next())
            {
                F[ *loop ] = alea.to<float>();
            }
        }

        dField D("D",L);
        {
            typename dField::Loop loop(F.lower,F.upper);
            for(loop.boot();loop.good();loop.next())
            {
                D[ *loop ] = alea.to<double>();
            }
        }

        vField V("V",L);
        {
            typename vField::Loop loop(L.lower,L.upper);
            for(loop.boot();loop.good();loop.next())
            {
                V[ *loop ] = alea.to<float>() * support::get<vf_t>();
            }
        }

        uField U("U",L);
        {
            typename uField::Loop loop(L.lower,L.upper);
            for(loop.boot();loop.good();loop.next())
            {
                U[ *loop ] = alea.to<double>() * support::get<vd_t>();
            }
        }


        VTK.writeField(fp,I,L);
        VTK.writeField(fp,F,L);
        VTK.writeField(fp,D,L);
        VTK.writeField(fp,V,L);
        VTK.writeField(fp,U,L);
        
        VTK.writeField(rp,I,L);
        VTK.writeField(rp,F,L);
        VTK.writeField(rp,D,L);
        VTK.writeField(rp,V,L);
        VTK.writeField(rp,U,L);

        VTK.writeField(cp,I,L);
        VTK.writeField(cp,F,L);
        VTK.writeField(cp,D,L);
        VTK.writeField(cp,V,L);
        VTK.writeField(cp,U,L);

        std::cerr << std::endl;

    }


}





Y_UTEST(vtk)
{
    
    vtk &VTK = vtk::instance();

    std::cerr << "-- Testing Writers" << std::endl;

    doVTK<short>();
    doVTK<unsigned short>();
    doVTK<float>();
    doVTK<double>();
    doVTK< complex<float> >();
    
    doVTK< complex<double> >();

    doVTK< point2d<float> >();
    doVTK< point2d<double> >();

    doVTK< point3d<float> >();
    doVTK< point3d<double> >();
    
    doVTK<mpn>();
    doVTK<mpz>();
    doVTK<mpq>();
    
    std::cerr << "format for int:   " << VTK.getNative<int>().format   << std::endl;
    std::cerr << "format for float: " << VTK.getNative<float>().format << std::endl;

    std::cerr << "-- Testing VTK files" << std::endl;
    


    doSave<Coord1D>();
    doSave<Coord2D>();
    doSave<Coord3D>();
    

}
Y_UTEST_DONE()
