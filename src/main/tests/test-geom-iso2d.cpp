#include "y/geometry/iso2d.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace geometry;

Y_UTEST(geom_iso2d)
{
    if(true)
    {
        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            const iso2d::coordinate c1(100 - unit_t( alea.leq(200) ),100 - unit_t( alea.leq(200) ), alea.choice() ? 1 : 0);
            const iso2d::coordinate c2(100 - unit_t( alea.leq(200) ),100 - unit_t( alea.leq(200) ), alea.choice() ? 1 : 0);

            Y_ASSERT(c1==c1);
            Y_ASSERT(c2==c2);

            const iso2d::edge_label a(c1,c1);
            const iso2d::edge_label b(c1,c2);
            const iso2d::edge_label c(c2,c1);
            Y_ASSERT(b==c);
            iso2d::edge_label::hasher H;
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
            const double r1 = math::__hypotenuse(yy,xx1);
            const double r2 = math::__hypotenuse(yy,xx2);
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

            iso2d::levels L;
            iso2d::scan( L, M, 1, nx, 1, ny, X, Y, z);
            std::cerr << "#L=" << L.size() << std::endl;
            for(size_t i=1;i<=L.size();++i)
            {
                std::cerr << "#L[" << i << "]=" << L[i]->size() << "/segments=" << L[i]->segments.size << std::endl;
                for( const iso2d::segment *s = L[i]->segments.head;s;s=s->next)
                {
                    fps("%g %g %u #0x%x\n",   s->a->vtx.x, s->a->vtx.y, unsigned(i), unsigned(L[i]->hash(s->a->tag)) );
                    fps("%g %g %u #0x%x\n\n", s->b->vtx.x, s->b->vtx.y, unsigned(i), unsigned(L[i]->hash(s->b->tag)) );
                }

                for( iso2d::shared_points::iterator j=L[i]->begin(); j!= L[i]->end(); ++j)
                {
                    const iso2d::unique_point &p = **j;
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
            const double r1 = math::__hypotenuse(yy,xx1);
            const double r2 = math::__hypotenuse(yy,xx2);
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

        iso2d::levels L;
        iso2d::scan( L, M, 1, nx, 1, ny, X, Y, z);
        std::cerr << "#L=" << L.size() << std::endl;

        iso2d::lines Lines;

        ios::ocstream fps("multi2d.dat");
        for(size_t i=1;i<=L.size();++i)
        {
            std::cerr << "@z=" << z[i] << std::endl;
            std::cerr << "#L[" << i << "]=" << L[i]->size() << "/segments=" << L[i]->segments.size << std::endl;
            for( const iso2d::segment *s = L[i]->segments.head;s;s=s->next)
            {
                fps("%g %g %u #0x%x\n",   s->a->vtx.x, s->a->vtx.y, unsigned(i), unsigned(L[i]->hash(s->a->tag)) );
                fps("%g %g %u #0x%x\n\n", s->b->vtx.x, s->b->vtx.y, unsigned(i), unsigned(L[i]->hash(s->b->tag)) );
            }

            Lines.connect(L[i]->segments);
            std::cerr << "#Lines=" << Lines.size << std::endl;
            {
                const string  filename = "line" + vformat("%g",z[i]) + ".dat";
                ios::ocstream fp(filename);
                unsigned count=1;
                for(const iso2d::line *l = Lines.head; l; l=l->next, ++count)
                {
                    fp << "#\n";
                    for(const iso2d::point *p = l->head;p;p=p->next)
                    {
                        fp("%g %g %u\n", (*p)->vtx.x, (*p)->vtx.y, count);
                    }
                    fp << "\n";
                }
            }
        }

    }

}
Y_UTEST_DONE()

