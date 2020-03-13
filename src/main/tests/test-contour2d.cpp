#include "y/geometry/iso2d/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/sort/unique.hpp"
#include "y/oxide/field2d.hpp"
#include "y/oxide/workspace.hpp"
#include "y/oxide/vtk.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace geometry;
using namespace Iso2D;

typedef point2d<float> p2d;

Y_UTEST(contour2d)
{
    {
        vector<Coordinate> coords;
        for(size_t i=100+alea.leq(1000);i>0;--i)
        {
            const Coordinate c(alea.range<unit_t>(-10,10),
                               alea.range<unit_t>(-10,10),
                               alea.__leq<short>(1));
            coords.push_back(c);
        }
        std::cerr << coords << std::endl;
        hsort(coords, Coordinate::Compare);
        std::cerr << coords << std::endl;

        for(size_t i=coords.size();i>1;--i)
        {
            const Coordinate &lo = coords[i-1];
            const Coordinate &hi = coords[i];
            Y_ASSERT(lo==lo);
            Y_ASSERT(hi==hi);
            Y_ASSERT(Coordinate::Compare(lo,lo)==0);
            Y_ASSERT(Coordinate::Compare(hi,hi)==0);
            switch(Coordinate::Compare(lo,hi))
            {
                case -1:
                case  0:
                    break;

                default:
                    throw exception("invalid Coordinate ordering");
            }
        }

        unique(coords);
        std::cerr << coords << std::endl;


        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            const Coordinate &a = coords[ alea.range<size_t>(1,coords.size()) ];
            const Coordinate &b = coords[ alea.range<size_t>(2,coords.size()) ];

            Edge eA(a);
            Edge eB(b);
            if(a==b)
            {
                Y_ASSERT(eA==eB);
            }
            else
            {
                Y_ASSERT(eA!=eB);
                Edge eAB(a,b);
                Edge eBA(b,a);
                Y_ASSERT(eAB==eBA);
            }

        }


    }
    
    unit_t resolution = 15;
    if( argc>1 )
    {
        resolution = string_convert::to<unit_t>(argv[1],"resolution");
    }

    
    {
        const Oxide::Layout2D            fullLayout( Oxide::Coord2D(1,1), Oxide::Coord2D(2*resolution,3*resolution) );
        Oxide::Workspace<Oxide::Coord2D> W(fullLayout,
                                           Oxide::Coord2D(1,1),
                                           0,
                                           Oxide::Coord2D(0,0),
                                           0);


        Oxide::Field2D<float>    & V  = W.create< Oxide::Field2D<float> >("V");
        const Oxide::AxisLayouts & Ax = W.axis[0];
        const Oxide::AxisLayouts & Ay = W.axis[1];

        Oxide::Field1D<float> x("x",Ax.outer);
        Oxide::Field1D<float> y("y",Ay.outer);

        Ax.mapBoundaries(x,-1.0f,1.0f);
        Ay.mapBoundaries(y,-1.0f,1.0f);


        // create dipolar field

        p2d p1(-0.5f,0);
        p2d p2( 0.7f,0);

        {
            Oxide::Layout2D::Loop loop(V.lower,V.upper);
            for(loop.boot();loop.good();loop.next())
            {
                const p2d p( x[loop.value.x], y[loop.value.y] );
                const float v1 = 1.0f/(sqrtf((p-p1).norm2())+0.1f);
                const float v2 = 1.0f/(sqrtf((p-p2).norm2())+0.1f);
                V(loop.value) = v1-v2;
            }
            {
                const string filename = "dipole.vtk";
                ios::ocstream fp(filename);

                const Oxide::vtk &VTK = Oxide::vtk::instance();
                VTK.writeHeader(fp);
                VTK.writeTitle(fp,filename);
                VTK.writeLayout(fp,V);
                VTK.writePointData(fp,V);
                VTK.writeField(fp,V,V);
            }
        }



        Contour::Levels z;
        z.insert(0);
        z.insert(-0.2);
        z.insert(-0.1);
        z.insert(1);
        z.insert(2);
        z.insert(-3);
        std::cerr << "z=" << z << std::endl;

        LevelSet ls;
        Scanner::Run(ls,V, x.lower, x.upper, y.lower, y.upper, x, y, z);

        {
            ios::ocstream pfp("lsp.dat");
            ios::ocstream sfp("lss.dat");
            for( LevelSet::iterator it = ls.begin(); it != ls.end(); ++it )
            {
                Level          &L = *it;
                const Segments  S = L->slist;
                L->buildLines(true);
                std::cerr << "Level #" << L->index << "@" << z[L->index] << std::endl;
                std::cerr << "    |_#points    = " << L->size()     << std::endl;
                std::cerr << "    |_#segments1 = " << S.size        << std::endl;
                std::cerr << "    |_#isolines  = " << L->iso.size() << std::endl;
                std::cerr << "    |_#segments2 = " << L->slist.size << std::endl;

                for( Points::iterator p = L->begin(); p != L->end(); ++p )
                {
                    const Vertex &v = (*p)->position;
                    pfp("%.15g %.15g %u\n", v.x, v.y, unsigned(L->index) );
                }

                for( const Segment *s = S.head; s; s=s->next)
                {
                    const Vertex &a = s->head->position;
                    const Vertex &b = s->tail->position;
                    sfp("%.15g %.15g %u\n", a.x, a.y, unsigned(L->index) );
                    sfp("%.15g %.15g %u\n", b.x, b.y, unsigned(L->index) );
                    sfp("\n");
                }

                {
                    const string  fn = vformat("iso%u.dat", unsigned(L->index) );
                    ios::ocstream fp(fn);
                    for(size_t i=1;i<=L->iso.size();++i)
                    {
                        const LineType &curve = *(L->iso[i]);
                        for(const Node *p = curve.head;p;p=p->next)
                        {
                            fp("%g %g %u\n",(*p)->position.x, (*p)->position.y, unsigned(i));
                        }
                        fp << '\n';
                    }
                }

            }
        }


        {
            Oxide::Layout2D::Loop loop(V.lower,V.upper);
            for(loop.boot();loop.good();loop.next())
            {
                const p2d p( x[loop.value.x], y[loop.value.y] );
                const float v1 = 1.0f/(sqrtf((p-p1).norm2())+0.1f);
                const float v2 = 1.0f/(sqrtf((p-p2).norm2())+0.1f);
                V(loop.value) = v1+v2;
            }
            {
                const string filename = "mount.vtk";
                ios::ocstream fp(filename);

                const Oxide::vtk &VTK = Oxide::vtk::instance();
                VTK.writeHeader(fp);
                VTK.writeTitle(fp,filename);
                VTK.writeLayout(fp,V);
                VTK.writePointData(fp,V);
                VTK.writeField(fp,V,V);
            }
        }

        z.free();
        z.insert(1.5);
        z.insert(2);
        z.insert(3);
        z.insert(6);

        Scanner::Run(ls,V, x.lower, x.upper, y.lower, y.upper, x, y, z);
        ls.buildLines(false);

        {
            for( LevelSet::iterator it = ls.begin(); it != ls.end(); ++it )
            {
                Level          &L = *it;
                std::cerr << "Level #" << L->index << "@" << z[L->index] << std::endl;
                std::cerr << "    |_#points    = " << L->size()     << std::endl;
                std::cerr << "    |_#isolines  = " << L->iso.size() << std::endl;


                {
                    const string  fn = vformat("isobis%u.dat", unsigned(L->index) );
                    ios::ocstream fp(fn);
                    for(size_t i=1;i<=L->iso.size();++i)
                    {
                        const LineType &curve = *(L->iso[i]);
                        for(const Node *p = curve.head;p;p=p->next)
                        {
                            fp("%g %g %u\n",(*p)->position.x, (*p)->position.y, unsigned(i));
                        }
                        fp << '\n';
                    }
                }
            }

        }

    }


}
Y_UTEST_DONE()

