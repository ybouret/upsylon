#include "y/geometry/iso2d.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ptr/shared.hpp"
#include "y/sequence/list.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace geometry;

namespace
{
    typedef Iso2d::Vertex vertex;

}

Y_UTEST(geom_iso2d)
{
    if(true)
    {
        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            const Iso2d::Coordinate c1(100 - unit_t( alea.leq(200) ),100 - unit_t( alea.leq(200) ), alea.choice() ? 1 : 0);
            const Iso2d::Coordinate c2(100 - unit_t( alea.leq(200) ),100 - unit_t( alea.leq(200) ), alea.choice() ? 1 : 0);

            Y_ASSERT(c1==c1);
            Y_ASSERT(c2==c2);

            const Iso2d::EdgeLabel a(c1,c1);
            const Iso2d::EdgeLabel b(c1,c2);
            const Iso2d::EdgeLabel c(c2,c1);
            Y_ASSERT(b==c);
            Iso2d::EdgeLabel::Hasher H;
            Y_ASSERT( H(b) == H(c) );


        }
    }

    std::cerr << "With dipole" << std::endl;

    const size_t   nx=60;
    const size_t   ny=80;
    matrix<double> M(ny,nx);
    vector<double> X(nx), Y(ny);
    for(size_t j=1;j<=ny;++j)
    {
        Y[j] = double(j-1)/(ny-1);
        const double yy = Y[j] - 0.5;
        for(size_t i=1;i<=nx;++i)
        {
            X[i] = double(i-1)/(nx-1);
            const double xx1 = X[i]-0.8;
            const double xx2 = X[i]-0.2;
            const double r1 = math::hypotenuse_of(yy,xx1);
            const double r2 = math::hypotenuse_of(yy,xx2);
            const double V  = 1.0/(r1+0.1)-1.0/(r2+0.1);
            M[j][i] = V;
        }
    }

    {
        vector<double> z(16,as_capacity);


        z.push_back(-5);
        z.push_back(-2);
        z.push_back(-0.1);
        z.push_back(0);
        z.push_back(0.1);
        z.push_back(2);
        z.push_back(5);


        std::cerr << "Contours for z=" << z << std::endl;
        {
            ios::ocstream fpp("iso2dp.dat");
            ios::ocstream fps("iso2ds.dat");

            Iso2d::Levels L;
            Iso2d::Scan( L, M, 1, nx, 1, ny, X, Y, z);
            std::cerr << "#L=" << L.size() << std::endl;
            for(size_t i=1;i<=L.size();++i)
            {
                std::cerr << "#L[" << i << "]=" << L[i]->size() << "/segments=" << L[i]->segments.size << std::endl;
                for( const Iso2d::Segment *s = L[i]->segments.head;s;s=s->next)
                {
                    fps("%g %g %u #0x%x\n",   s->a->vtx.x, s->a->vtx.y, unsigned(i), unsigned(L[i]->hash(s->a->tag)) );
                    fps("%g %g %u #0x%x\n\n", s->b->vtx.x, s->b->vtx.y, unsigned(i), unsigned(L[i]->hash(s->b->tag)) );
                }

                for( Iso2d::SharedPoints::iterator j=L[i]->begin(); j!= L[i]->end(); ++j)
                {
                    const Iso2d::UniquePoint &p = **j;
                    fpp("%g %g %u #0x%08x\n",p.vtx.x, p.vtx.y, unsigned(i), unsigned(L[i]->hash(p.tag)) );
                }

            }
        }

        std::cerr << "plot 'iso2ds.dat' with lines linecolor variable" << std::endl;
    }


    std::cerr << "With multiple same level..." << std::endl;
    for(size_t j=1;j<=ny;++j)
    {
        const double yy = Y[j] - 0.5;
        for(size_t i=1;i<=nx;++i)
        {
            const double xx1 = X[i]-0.8;
            const double xx2 = X[i]-0.2;
            const double r1 = math::hypotenuse_of(yy,xx1);
            const double r2 = math::hypotenuse_of(yy,xx2);
            const double V  = 1.0/(r1+0.1)+1.0/(r2+0.1);
            M[j][i] = V;
        }
    }

    {
        vector<double> z;
        z.push_back(4);
        z.push_back(5);
        z.push_back(6);
        z.push_back(8);

        Iso2d::Levels L;
        Iso2d::Scan( L, M, 1, nx, 1, ny, X, Y, z);
        std::cerr << "#L=" << L.size() << std::endl;


        {
            // converting each level into lines
            Iso2d::Lines lines;

            ios::ocstream  fps("multi2d.dat");
            vector<vertex> curve;
            for(size_t i=1;i<=L.size();++i)
            {
                std::cerr << "@z=" << z[i] << std::endl;
                std::cerr << "#L[" << i << "]=" << L[i]->size() << "/segments=" << L[i]->segments.size << std::endl;
                for( const Iso2d::Segment *s = L[i]->segments.head;s;s=s->next)
                {
                    fps("%g %g %u #0x%x\n",   s->a->vtx.x, s->a->vtx.y, unsigned(i), unsigned(L[i]->hash(s->a->tag)) );
                    fps("%g %g %u #0x%x\n\n", s->b->vtx.x, s->b->vtx.y, unsigned(i), unsigned(L[i]->hash(s->b->tag)) );
                }

                lines.connect(L[i]->segments);
                std::cerr << "#Lines=" << lines.size << std::endl;
                {
                    const string  filename = "line" + vformat("%g",z[i]) + ".dat";
                    ios::ocstream fp(filename);
                    {
                        unsigned count=1;
                        for(const Iso2d::Line *l = lines.head; l; l=l->next, ++count)
                        {
                            l->compile_to(curve);
                            std::cerr << "|_\t->line#" << count << ": size=" << l->size << "/" << curve.size() << std::endl;
                            fp << "#\n";
                            for(const Iso2d::Point *p = l->head;p;p=p->next)
                            {
                                fp("%g %g %u\n", (*p)->vtx.x, (*p)->vtx.y, count);
                            }
                            fp << "\n";
                        }
                    }
                }

                {
                    // converting lines into curves
                    list< shared_ptr< vector<vertex> > > curves;
                    lines.compile_to(curves);
                    std::cerr << "|_#curves=" << curves.size() << std::endl;
                    for(size_t i=1;i<=curves.size();++i)
                    {
                        shared_ptr< vector<vertex> > &c = curves[i];
                        std::cerr << "  |_curve#" << i << " : " << c->size() << std::endl;
                    }
                }
            }
        }

        {
            // converting points into level_set
            Iso2d::LevelSet ls;
            Iso2d::convert(ls,L);
            std::cerr << "#ls=" << ls.size() << std::endl;
            for(size_t i=1;i<=ls.size();++i)
            {
                std::cerr << "|_ls[" << i << "]=" << ls[i]->size() << std::endl;
                const Iso2d::Curves &C = *ls[i];
                for(size_t j=1;j<=C.size();++j)
                {
                    std::cerr << " |_ls[" << i <<"][" << j << "]=" << C[j]->size() << std::endl;
                }
            }
        }

    }


    //
    for(size_t j=1;j<=ny;++j)
    {
        const double yy  = Y[j];
        double       yy1 = yy-0.25;
        double       yy2 = yy-0.75;
        for(size_t i=1;i<=nx;++i)
        {
            const double xx1 = X[i]-0.25;
            const double xx2 = X[i]-0.75;
            const double r1 = math::hypotenuse_of(xx1,yy1);
            const double r2 = math::hypotenuse_of(xx2,yy1);
            const double r3 = math::hypotenuse_of(xx2,yy2);
            const double r4 = math::hypotenuse_of(xx1,yy2);
            const double V  = 1.0/(r1+0.1)-1.0/(r2+0.1)+1.0/(r3+0.1)-1.0/(r4+0.1);
            M[j][i] = V;
        }
    }

    {
        vector<double,Iso2d::Allocator> z;
        z.push_back(-2);
        z.push_back(-1);
        z.push_back(-0.5);
        z.push_back(0);
        z.push_back(0.5);
        z.push_back(1);
        z.push_back(2);
        Iso2d::LevelSet ls;
        Iso2d::Build(ls, M, 1, nx, 1, ny, X, Y, z);
        ios::ocstream fp("quad2d.dat");
        for(size_t i=1;i<=ls.size();++i)
        {
            const Iso2d::Curves &curves = *ls[i];
            for(size_t j=1;j<=curves.size();++j)
            {
                const Iso2d::Curve &curve = *curves[j];
                for(size_t k=1;k<=curve.size();++k)
                {
                    fp("%g %g %u\n", curve[k].x, curve[k].y, unsigned(i) );
                }
                fp("\n");
            }
        }
    }


}
Y_UTEST_DONE()

