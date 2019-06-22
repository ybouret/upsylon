
//! \file
#ifndef Y_GEOMETRIC_CURVE_INCLUDED
#define Y_GEOMETRIC_CURVE_INCLUDED 1


#include "y/math/geometric/points.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    
    namespace math
    {

        namespace Geometric
        {

            template <typename T, template <class> class POINT, class ALLOCATOR=memory::global>
            class Curve
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef PointInfoFor<T,POINT>    PointInfo;
                typedef typename PointInfo::Type PointType; //!< user point type
                typedef typename PointInfo::Core CorePoint; //!< matching point[2|3]d<mutable_type>

                class Node
                {
                public:
                    CorePoint    t;         //!< tangent
                    CorePoint    n;         //!< normal
                    mutable_type curvature; //!< curvature

                    inline  Node() throw() :  t(), n(), curvature(0) {}
                    inline  Node(const CorePoint tt) throw() : t(tt), n(), curvature(0) {}
                    inline ~Node() throw() {}
                    inline  Node(const Node &node) throw() :   t(node.t), n(node.n), curvature(node.curvature) {}

                private:
                    Y_DISABLE_ASSIGN(Node);
                };
                typedef vector<Node,ALLOCATOR> Nodes;
                Nodes nodes;

                inline explicit Curve() : nodes() {}
                inline virtual ~Curve() throw() {}

                template <typename SEQUENCE>
                inline void compute( const SEQUENCE &points, const Boundaries boundaries )
                {
                    const size_t n = points.size();
                    nodes.free();
                    nodes.ensure(n);
                    switch(n)
                    {
                        case 0:
                        case 1:  break;
                        case 2:  compute_two(points,boundaries); break;
                        default: compute_all(points,boundaries); break;
                    }
                    assert(nodes.size()==n);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curve);

                template <typename SEQUENCE>
                void compute_two( const SEQUENCE &points, const Boundaries boundaries )
                {
                    const CorePoint A = PointInfo::Type2Core( points.front() );
                    const CorePoint B = PointInfo::Type2Core( points.back()  );
                    const CorePoint AB(A,B);
                    const_type      ab2 = AB.norm2();
                    if(ab2<=0) throw libc::exception( EDOM, "null norm" );
                    Node node(AB/sqrt_of(ab2));
                    nodes.push_back_(node);
                    switch(boundaries)
                    {
                        case Standard: nodes.push_back_(node);                   break;
                        case Periodic: node.t = -node.t; nodes.push_back_(node); break;
                    }

                }

                inline void compute_head(const PointType &PA, const PointType &PB, const PointType &PC)
                {
                    const CorePoint A = PointInfo::Type2Core(PA);
                    const CorePoint B = PointInfo::Type2Core(PB);
                    const CorePoint C = PointInfo::Type2Core(PC);
                    Node            node;

                    nodes.push_back_(node);

                }

                inline void compute_tail(const PointType &PA, const PointType &PB, const PointType &PC)
                {
                    const CorePoint A = PointInfo::Type2Core(PA);
                    const CorePoint B = PointInfo::Type2Core(PB);
                    const CorePoint C = PointInfo::Type2Core(PC);
                    
                    Node node;

                    nodes.push_back_(node);
                }

                inline void compute_bulk( const PointType &PA, const PointType &PB, const PointType &PC )
                {
                    const CorePoint A = PointInfo::Type2Core(PA);
                    const CorePoint B = PointInfo::Type2Core(PB);
                    const CorePoint C = PointInfo::Type2Core(PC);

                    const CorePoint AC(A,C);
                    const_type      ac2 = AC.norm2();
                    if(ac2<=0) throw libc::exception( EDOM, "null norm" );

                    Node node;
                    const_type ac = sqrt_of(ac2);
                    node.t = AC/sqrt_of(ac2);


                    const CorePoint W = A+C-(B+B);
                    node.curvature = 4 * CorePoint::det(AC,W)/(ac*ac*ac);
                    
                    nodes.push_back_(node);
                }

                template <typename SEQUENCE>
                void compute_all( const SEQUENCE &points, const Boundaries boundaries )
                {
                    const size_t   n = points.size(); assert(n>=3);
                    const typename SEQUENCE::const_iterator head = points.begin();
                    typename       SEQUENCE::const_iterator prev = head;
                    typename       SEQUENCE::const_iterator curr = head;
                    ++curr;
                    typename       SEQUENCE::const_iterator next = curr;
                    ++next;

                    const typename SEQUENCE::const_reverse_iterator tail = points.rbegin();
                    // head
                    switch(boundaries)
                    {
                        case Periodic: compute_bulk(*tail,*prev,*curr); break;
                        case Standard: compute_head(*prev,*curr,*next); break;
                    }

                    // bulk
                    for(size_t i=2;i<n;++i,++prev,++curr,++next)
                    {
                        compute_bulk(*prev,*curr,*next);
                    }

                    // tail
                    switch(boundaries)
                    {
                        case Periodic: compute_bulk(*prev,*curr,*head); break;
                        case Standard: next=prev; --prev; compute_tail(*next,*prev,*curr); break;
                    }
                }

            };

            

        }
        
    }
    
}

#endif

