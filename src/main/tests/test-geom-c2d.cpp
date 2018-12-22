#include "y/geometry/contour2d.hpp"
#include "y/container/matrix.hpp"
#include "y/ipso/field2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace geometry;

Y_UTEST(geom_c2d)
{
    contour2d::shared_point_db pdb;

    pdb.free();

    for(size_t i=10+alea.leq(10000);i>0;--i)
    {
        const contour2d::offset       d( alea.partial<contour2d::resolution>(), alea.partial<contour2d::resolution>() );
        const contour2d::coordinate   c( alea.partial<unit_t>(), alea.partial<unit_t>() );
        const contour2d::point        p( alea.to<double>(), alea.to<double>() );
        const contour2d::identifier   id(c,d);
        const contour2d::shared_point sp = new contour2d::unique_point(id,p);
        if(!pdb.insert(sp))
        {
            std::cerr << "multiple point detected!" << std::endl;
        }
    }
    std::cerr << "#points=" << pdb.size() << std::endl;

    contour2d::segments segs(0);
    for(size_t i=10+alea.leq(100);i>0;--i)
    {
        const contour2d::shared_point A = pdb.fetch( alea.lt( pdb.size() ) );
        const contour2d::shared_point B = pdb.fetch( alea.lt( pdb.size() ) );
        segs.push_back( new contour2d::segment(A,B) );
    }

    std::cerr << "segs=" << segs.size << std::endl;

    pdb.release();

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

    vector<double> z(16,as_capacity);
    //z[1] = -2;
    //z[2] =  0;
    //z[3] =  2;

    z.push_back(-5);
    z.push_back(-2);
    z.push_back(-0.1);
    z.push_back(0);
    z.push_back(0.1);
    z.push_back(2);
    z.push_back(5);

    pdb.release();
    std::cerr << "Contours for z=" << z << std::endl;
    contour2d::shared_segments_db sdb;
    contour2d::scan(sdb, M, 1, nx, 1, ny, X, Y, z, pdb);

    std::cerr << "#unique_points=" << pdb.size() << std::endl;
    std::cerr << "#contours     =" << sdb.size() << std::endl;

    {
        ios::ocstream fp("c2d.dat");
        for(contour2d::shared_segments_db::iterator i=sdb.begin();i!=sdb.end();++i)
        {
            const contour2d::segments &S = **i;
            std::cerr << "Segment[" << S.indx << "].size= " << S.size << "\t@" << z[S.indx];
            for( const contour2d::segment *s = S.head; s;s=s->next )
            {
                const contour2d::unique_point &a = *(s->a), &b = *(s->b);
                //std::cerr << " " << a.pos << "->" << b.pos << std::endl;
                fp("%g %g\n",   a.pos.x,a.pos.y);
                fp("%g %g\n\n", b.pos.x,b.pos.y);
            }
            std::cerr << std::endl;
        }
    }

    {
        std::cerr << "Building IsoLines" << std::endl;
        for(contour2d::shared_segments_db::iterator i=sdb.begin();i!=sdb.end();++i)
        {
            const contour2d::segments &S = **i;
            std::cerr << "Segment[" << S.indx << "].size= " << S.size << "\t@" << z[S.indx] << std::endl;
            contour2d::iso_lines       lines;
            S.build_connected(lines);
            std::cerr << "#isoLines=" << lines.size << std::endl;

        }

    }

}
Y_UTEST_DONE()

