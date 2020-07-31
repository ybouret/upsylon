#include "y/mkl/euclidean/arc/standard.hpp"
#include "y/mkl/euclidean/arc/periodic.hpp"
#include "y/utest/run.hpp"
#include "y/associative/set.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/mkl/fcn/integrate.hpp"

using namespace upsylon;
using namespace mkl;
using namespace Euclidean;


namespace {



    template <typename T,template <class> class VTX>
    struct euclidean_test
    {
        Y_EUCLIDEAN_ARC_TYPES();

        typedef set<PointKey,SharedPoint> Points;




        static inline
        void save_pos( const string &pfx, const ArcType &arc, const bool loop )
        {
            const string  fn = pfx + ".dat";
            ios::ocstream fp(fn);
            for(size_t i=1;i<=arc.nodes.size();++i)
            {
                PointType::Print(fp, arc.nodes[i]->P ) << '\n';
            }
            if(loop)
            {
                PointType::Print(fp, arc.nodes[1]->P ) << '\n';
            }
        }

        static inline
        void save_v( const string &pfx, const ArcType &arc )
        {
            const string  fn = pfx + "_v.dat";
            ios::ocstream fp(fn);
            for(size_t i=1;i<=arc.nodes.size();++i)
            {
                const NodeType &node = *arc.nodes[i];
                vertex p = node.P;
                PointType::Print(fp,p) << '\n';
                p += node.V/2;
                PointType::Print(fp,p) << '\n' << '\n';
            }
        }


        static inline
        void save_a( const string &pfx, const ArcType &arc )
        {
            const string  fn = pfx + "_a.dat";
            ios::ocstream fp(fn);
            for(size_t i=1;i<=arc.nodes.size();++i)
            {
                const NodeType &node = *arc.nodes[i];
                vertex p = node.P;
                PointType::Print(fp,p) << '\n';
                p += node.A/2;
                PointType::Print(fp,p) << '\n' << '\n';
            }
        }

        static inline
        void save_c( const string &pfx, const ArcType &arc )
        {
            const string  fn = pfx + "_c.dat";
            ios::ocstream fp(fn);
            for(size_t i=1;i<=arc.nodes.size();++i)
            {
                const NodeType &node = *arc.nodes[i];
                vertex p = node.P;
                PointType::Print(fp,p) << '\n';
                p += node.basis.n * node.basis.curvature/2;
                PointType::Print(fp,p) << '\n' << '\n';
            }
        }



        static inline
        void save_class(const string  &pfx,
                        ArcType       &arc,
                        const ArcClass C,
                        const_type     nmax)
        {
            arc.motion(C);
            string fn = pfx;
            switch (C) {
                case Arc0: fn += "_i0"; break;
                case Arc1: fn += "_i1"; break;
                case Arc2: fn += "_i2"; break;
            }
            ios::ocstream fp( fn + ".dat" );
            for(type i=1;i<=nmax;i+=const_type(0.02))
            {
                vertex p;
                arc.compute(i, &p, 0, 0);
                PointType::Print(fp,p) << '\n';
            }
        }

        static inline
        void save_classes(const string  &pfx,
                          ArcType       &arc,
                          const_type     nmax)
        {
            save_class(pfx,arc,Arc0,nmax);
            save_class(pfx,arc,Arc1,nmax);
            save_class(pfx,arc,Arc2,nmax);
        }


        static inline
        void make(const char *TID, const char *PID, const size_t nn)
        {
            std::cerr << PID << "<" << TID << "> in " << Dimensions << "D" << std::endl;

            const size_t np = (nn>0) ? nn : 10 + alea.leq(30);
            std::cerr << "With " << np << " points" << std::endl;

            const_type noise = const_type(0.1);
            Points     points(np,as_capacity);

            StandardArc<T,VTX> sa;
            PeriodicArc<T,VTX> pa;

            for(size_t i=0;i<np;++i)
            {
                const_type theta = (numeric<mutable_type>::two_pi * i)/np;
                //const_type rho   = fabs_of( 2 * cos_of( theta+theta ) );
                //const_type x     = rho * cos_of(theta) + noise * alea.symm<T>();
                //const_type y     = rho * sin_of(theta) + noise * alea.symm<T>();
                const_type x     = type(1.4) * cos_of(theta) + noise * alea.symm<T>();
                const_type y     = sin_of(theta)   + noise * alea.symm<T>();
                const_type z     = sin_of(theta/2) + noise*alea.symm<T>();
                const_type arr[4] = {x,y,z,0};
                const_vertex &v = *aliasing::as<const_vertex>(arr);
                //std::cerr << "v=" << v << std::endl;
                const SharedPoint p = new PointType(v);
                Y_ASSERT( points.insert(p) );
                const SharedNode  n = new NodeType(p);

                sa << p;
                pa << p;
            }

            string std_pfx = "std_"; std_pfx << PID << '_' << TID;
            string per_pfx = "per_"; per_pfx << PID << '_' << TID;

            save_pos(std_pfx,sa,false);
            save_pos(per_pfx,pa,true);


 
            sa.motion(Arc0);
            pa.motion(Arc0);
            


            //typename numeric<type>::function Ls( &sa, & Arc<T,VTX>::speed );
            //typename numeric<type>::function Lp( &sa, & Arc<T,VTX>::speed );

            save_v(std_pfx,sa);
            save_v(per_pfx,pa);

            sa.motion(Arc1);
            sa.motion(Arc2);

            pa.motion(Arc1);
            pa.motion(Arc2);

            save_a(std_pfx,sa);
            save_a(per_pfx,pa);

            save_c(std_pfx,sa);
            save_c(per_pfx,pa);


            save_classes(std_pfx,sa,type(np));
            save_classes(std_pfx,sa,type(np));
            save_classes(std_pfx,sa,type(np));

            save_classes(per_pfx,pa,type(np+1));
            save_classes(per_pfx,pa,type(np+1));
            save_classes(per_pfx,pa,type(np+1));



        }

    };

}

#define _EUCLIDEAN(TYPE,POINT) euclidean_test<TYPE,POINT>::make(#TYPE,#POINT,nn)

#include "y/string/convert.hpp"

Y_UTEST(euclidean)
{

    size_t nn = 0;
    if(argc>1)
    {
        nn = string_convert::to<size_t>( argv[1], "nn" );
    }
#if 1
    _EUCLIDEAN(float,point2d);
    _EUCLIDEAN(double,point2d);

    _EUCLIDEAN(float,point3d);
    _EUCLIDEAN(double,point3d);
#endif

}
Y_UTEST_DONE()



