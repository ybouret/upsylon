
#include "y/geometry/closest.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/type/point3d.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {

    template <typename POINT>
    typename POINT::type dsq( const POINT &a, const POINT &b ) throw()
    {
        typename POINT::type ans = 0;
        for(size_t i=POINT::DIMS;i>0;--i)
        {
            ans += square_of( a[i] - b[i] );
        }
        return ans;
    }

//#define NL 4
#define NL 1+alea.leq(100)

    template <typename NODE>
    void doFind( concurrent::for_each &loop, const char *fn)
    {
        typedef typename NODE::type               point_type;
        typedef typename point_type::mutable_type type;
        typedef typename point_type::const_type   const_type;

        core::list_of_cpp<NODE> lhs;
        core::list_of_cpp<NODE> rhs;

        for( size_t i=NL; i>0; --i )
        {
            const_type x = alea.range<type>(0,10);
            const_type y = alea.range<type>(0,10);
            point_type p(x,y);
            lhs.push_back( new NODE(p) );
        }

        for( size_t i=NL; i>0; --i )
        {
            const_type x = alea.range<type>(11,20);
            const_type y = alea.range<type>(11,20);
            point_type p(x,y);
            rhs.push_back( new NODE(p) );
        }

        std::cerr << "#L=" << lhs.size << ", #R=" << rhs.size << std::endl;
        typename geometry::closest<NODE>::info opt;
        geometry::closest<NODE>::find(opt, lhs, rhs, loop, dsq<point_type> );
        std::cerr << "opt=" << opt.d << " between " << (**opt.l) << " and " << (**opt.r) << std::endl;


        string filename = fn;
        {
            ios::ocstream fp(filename);
            for(const NODE *node = lhs.head; node; node=node->next)
            {
                fp("%g %g\n", double( (**node).x ), double ( (**node).y ) );
            }

            for(const NODE *node = rhs.head; node; node=node->next)
            {
                fp("%g %g\n", double( (**node).x ), double ( (**node).y ) );
            }
        }

        {
            ios::ocstream fp( "opt-" + filename );
            fp("%g %g\n", double( (**opt.l).x ), double ( (**opt.l).y) );
            fp("%g %g\n", double( (**opt.r).x ), double ( (**opt.r).y) );

        }
    }

}

Y_UTEST(closest)
{
    concurrent::simd loop;

    doFind< core::cpp_node_of< point2d<float>  >  >(loop,"p2f.dat");
    doFind< core::cpp_node_of< point3d<double> >  >(loop,"p3f.dat");
    doFind< core::cpp_node_of< point2d<unit_t> >  >(loop,"p2i.dat");

}
Y_UTEST_DONE()

