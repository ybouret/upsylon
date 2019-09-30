#include "y/geometry/iso3d/edge.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {


            Edge::~Edge() throw()
            {
            }
            
            Edge:: Edge( const Coordinate &C ) throw() :
            lower(C),
            upper(C)
            {
            }

            Edge:: Edge(const Coordinate &a,
                        const Coordinate &b) throw() :
            lower(a),
            upper(b)
            {
                assert(a!=b);
                if( Coordinate::Compare(a, b) >= 0 )
                {
                    bswap(lower,upper);
                }
                assert( Coordinate::Compare(lower,upper) < 0 );
            }

            Edge:: Edge( const Edge &_) throw() :
            lower( _.lower ),
            upper( _.upper )
            {
            }

            int Edge:: Compare(const Edge &lhs, const Edge &rhs) throw()
            {
                assert(Coordinate::Compare(lhs.lower,lhs.upper) <= 0 );
                assert(Coordinate::Compare(rhs.lower,rhs.upper) <= 0 );
                return comparison::increasing_lexicographic(&lhs.lower, &rhs.lower, 2);
            }
            
            bool operator==( const Edge &lhs, const Edge &rhs ) throw()
            {
                return lhs.lower==rhs.lower && lhs.upper==rhs.upper;
            }
            
            bool operator!=( const Edge &lhs, const Edge &rhs ) throw()
            {
                return lhs.lower!=rhs.lower || lhs.upper!=rhs.upper;
            }
            
            bool operator<(const Edge &lhs, const Edge &rhs) throw()
            {
                return Edge::Compare(lhs,rhs)<0;
            }
            
            
            void Edge:: Sort3(Edge &a0, Edge &a1, Edge &a2) throw()
            {
                if( Compare(a2,a1)<0 ) bswap(a1,a2);
                if( Compare(a2,a0)<0 ) bswap(a0,a2);
                if( Compare(a1,a0)<0 ) bswap(a0,a1);
                assert( Compare(a0,a1) <= 0);
                assert( Compare(a1,a2) <= 0);
                assert( Compare(a0,a2) <= 0);
            }
            

            Edge:: Hasher:: Hasher() throw() : H() {}
            Edge:: Hasher:: ~Hasher() throw() {}

            size_t Edge::Hasher::operator()(const Edge &_) throw()
            {
                H.set();
                _.lower.__run(H);
                _.upper.__run(H);
                return H.key<size_t>();
            }

            Edge3:: ~Edge3() throw()
            {
            }
            
            Edge3:: Edge3( const Edge3 &_) throw() :
            e0(_.e0),
            e1(_.e1),
            e2(_.e2)
            {
            }
            
            Edge3:: Edge3( const Edge &E0, const Edge &E1, const Edge &E2 ) throw() :
            e0(E0),
            e1(E1),
            e2(E2)
            {
                Edge::Sort3((Edge &)e0,
                            (Edge &)e1,
                            (Edge &)e2);
            }
            
            bool operator==( const Edge3 &lhs, const Edge3 &rhs ) throw()
            {
                return lhs.e0==rhs.e0 && lhs.e1==rhs.e1 && lhs.e2==rhs.e2;
            }
            
            int  Edge3:: Compare(const Edge3 &lhs, const Edge3 &rhs) throw()
            {
                return comparison::increasing_lexicographic(&lhs.e0, &rhs.e0, 3);
            }
            
        }

    }

}
