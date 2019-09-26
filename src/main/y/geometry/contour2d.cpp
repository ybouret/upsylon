
#include "y/geometry/contour2d.hpp"
#include "y/type/bzset.hpp"
#include "y/comparison.hpp"
#include "y/type/bswap.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace geometry {

        double contour:: average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_abs(arr) * 0.25;
        }

        contour::sign_type contour:: sign_of( const double value ) throw()
        {
            if(value<0.0)
            {
                return is_negative;
            }
            else if(0.0<value)
            {
                return is_positive;
            }
            else
            {
                return is_zero;
            }
        }

    }

}


namespace upsylon {

    namespace geometry {

        contour2d:: coordinate:: coordinate(const unit_t I,
                                            const unit_t J,
                                            const unit_t Q) throw() :
        i(I), j(J), q(Q)
        {
        }

        contour2d:: coordinate:: ~coordinate() throw()
        {
            bzset_(i);
            bzset_(j);
            bzset_(q);
        }

        contour2d:: coordinate:: coordinate( const coordinate &_ ) throw() :
        i( _.i ),
        j( _.j ),
        q( _.q )
        {
        }

        void contour2d:: coordinate:: __run(hashing::function &H) const throw()
        {
            H.run_type(i);
            H.run_type(j);
            H.run_type(q);
        }


        int  contour2d:: coordinate:: compare(const coordinate &lhs, const coordinate &rhs) throw()
        {
            return comparison::increasing_lexicographic(&lhs.i,&rhs.i,3);
        }

        bool operator==(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
        }

        bool operator!=(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i!=rhs.i) || (lhs.j!=rhs.j) || (lhs.q!=rhs.q);
        }

        std::ostream & operator<<(std::ostream &os,const contour2d::coordinate&c)
        {
            os << '(' << c.i << ',' << c.j << ':' << c.q << ')';
            return os;
        }

        bool operator<(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return contour2d::coordinate::compare(lhs,rhs) < 0;
        }

    }

}

namespace upsylon {

    namespace geometry {


        contour2d:: edge:: ~edge() throw()
        {
        }

        contour2d:: edge:: edge( const coordinate &single) throw() :
        lower(single),
        upper(single)
        {
        }

        contour2d:: edge:: edge( const coordinate &a, const coordinate &b) throw() :
        lower(a),
        upper(b)
        {
            assert(a!=b);
            if( coordinate::compare(lower,upper) > 0 )
            {
                bswap(lower,upper);
            }
            assert(coordinate::compare(lower,upper)<0);

        }

        bool operator==(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
        }

        bool operator!=(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower!=rhs.lower) || (lhs.upper!=rhs.upper);
        }

        contour2d:: edge:: hasher::  hasher() throw() : H() {}
        contour2d:: edge:: hasher:: ~hasher() throw()  {}

        size_t contour2d:: edge:: hasher::operator()(const edge &_) throw()
        {
            H.set();
            _.lower.__run(H);
            _.upper.__run(H);
            return H.key<size_t>();
        }

    }

}

namespace upsylon {

    namespace geometry {

        const contour2d::edge & contour2d:: point:: key() const throw()
        {
            return location;
        }

    }

}

#include "y/exception.hpp"

namespace upsylon {

    namespace geometry {

        const size_t & contour2d:: unique_points:: key() const throw()
        {
            return index;
        }

        contour2d:: unique_points:: unique_points( const size_t i ) throw() :
        index(i)
        {

        }

        contour2d:: unique_points:: ~unique_points() throw()
        {
        }

        void contour2d:: createDB(unique_points_levels  &db, const size_t n)
        {
            db.free();
            db.ensure(n);
            for(size_t k=1;k<=n;++k)
            {
                const unique_points_level level = new unique_points(k);
                if(!db.insert(level))
                {
                    throw exception("contour2d: unexpected failure to create level#%u", unsigned(k) );
                }
            }
            assert(n==db.size());
        }


    }

}


namespace upsylon {

    namespace geometry {

        namespace {
            static const unsigned n0 = 0x0001;
            static const unsigned z0 = 0x0002;
            static const unsigned p0 = 0x0004;

            static const unsigned n1 = 0x0008;
            static const unsigned z1 = 0x0010;
            static const unsigned p1 = 0x0020;

            static const unsigned n2 = 0x0040;
            static const unsigned z2 = 0x0080;
            static const unsigned p2 = 0x0100;
        }

        
#define Y_CONTOUR2D_FLAG(I) do { \
switch(contour::sign_type(d##I))\
{\
case contour::is_negative:  flags |= n##I; break;\
case contour::is_zero:      flags |= z##I; break;\
case contour::is_positive:  flags |= p##I; break;\
} } while(false)

        void contour2d:: scan_triangles(unique_points_levels    &db,
                                        const size_t             k,
                                        const coordinate         *c,
                                        const double             *d,
                                        const vertex             *v)
        {
            unique_points_level *pup      = db.search(k); assert(pup);
            unique_points_level &upoints = *pup;

            static const unsigned i0 = 0;
            static const unsigned indices[4][2] =
            {
                {1,2}, {2,3}, {3,4}, {4,1}
            };
            for(size_t t=0;t<4;++t)
            {
                const unsigned i1 = indices[t][0];
                const unsigned i2 = indices[t][1];
                const double   d0 = d[i0];
                const double   d1 = d[i1];
                const double   d2 = d[i2];

                unsigned flags = 0;
                Y_CONTOUR2D_FLAG(0);
                Y_CONTOUR2D_FLAG(1);
                Y_CONTOUR2D_FLAG(2);

                switch(flags)
                {
                    case z0|p1|n2:
                    case z0|n1|p2:
                        break;

                        
                    default:
                        break;
                }

            }
        }


    }

}
