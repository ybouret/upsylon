#include "y/geometry/iso3d/edge.hpp"
#include "y/comparison.hpp"
#include "y/type/block/swap.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/cswap.hpp"

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
                if( Coordinate::Compare(a,b) >= 0 )
                {
                    bswap( aliasing::_(lower), aliasing::_(upper) );
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
            

#if 0
            void Edge:: Sort3(Edge &a0, Edge &a1, Edge &a2) throw()
            {
                if( Compare(a2,a1)<0 ) bswap(a1,a2);
                if( Compare(a2,a0)<0 ) bswap(a0,a2);
                if( Compare(a1,a0)<0 ) bswap(a0,a1);
                assert( Compare(a0,a1) <= 0);
                assert( Compare(a1,a2) <= 0);
                assert( Compare(a0,a2) <= 0);
            }
#endif

            void Edge:: Sort3(Edge **arr) throw()
            {
                assert(arr);
                if( Compare(*arr[2],*arr[1]) < 0 ) cswap<Edge*>( arr[2], arr[1] );
                if( Compare(*arr[2],*arr[0]) < 0 ) cswap<Edge*>( arr[2], arr[0] );
                if( Compare(*arr[1],*arr[0]) < 0 ) cswap<Edge*>( arr[1], arr[0] );
                assert( Compare(*arr[0],*arr[1]) <= 0);
                assert( Compare(*arr[1],*arr[2]) <= 0);
                assert( Compare(*arr[0],*arr[2]) <= 0);
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
                memset(edge,0,sizeof(edge));
            }

            Edge3:: Edge3( const Edge &E0, const Edge &E1, const Edge &E2 ) throw() :
            edge()
            {
                assert(E0!=E1);
                assert(E0!=E2);
                assert(E1!=E2);
                memset(edge,0,sizeof(edge));
                edge[0] = &E0;
                edge[1] = &E1;
                edge[2] = &E2;
                Edge::Sort3((Edge**)edge);
            }

            bool operator==( const Edge3 &lhs, const Edge3 &rhs ) throw()
            {
                for(size_t i=0;i<3;++i)
                {
                    if( lhs.edge[i] != rhs.edge[i] ) return false;
                }
                return true;
            }

            void Edge3:: __run(hashing::function &H) const throw()
            {
                H.run( edge, sizeof(edge) );
            }


            Edge3:: Hasher:: Hasher() throw() : H() {}
            Edge3:: Hasher:: ~Hasher() throw() {}

            size_t Edge3::Hasher::operator()(const Edge3 &_) throw()
            {
                H.set();
                _.__run(H);
                return H.key<size_t>();
            }

        }

    }

}
