//! \file
#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1


#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/type/bzset.hpp"
#include "y/sequence/vector.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace math
    {

        //! curve for 2D/3D points
        struct curve
        {
            enum boundaries
            {
                standard,
                periodic
            };
            //! internal node
            template <typename T, template <class> class POINT>
            class node
            {
            public:
                typedef POINT<T>    point_type; //!< internal point type
                static const size_t DIM = sizeof(point_type)/sizeof(T);
                point_type r; //!< position
                point_type t; //!< tangent
                point_type n; //!< normal
                T          C; //!< curvature

                inline  node() throw() : r(), t(), n(), C(0) {}                         //!< setup, all zero
                inline ~node() throw() {}                                               //!< cleanup
                inline  node(const node &o) throw() : r(o.r), t(o.t), n(o.n), C(o.C) {} //!< copy
                inline  node(const point_type &p) throw() : r(p), t(), n(), C(0) {}     //!< setup position

                inline void reset() throw() { bzset(t); bzset(n); C=0; }

            private:
                Y_DISABLE_ASSIGN(node);
            };

            //! array of points
            template <typename T, template <class> class POINT>
            class points : public vector< node<T,POINT> >
            {
            public:
                Y_DECL_ARGS(T,type);
                typedef node<T,POINT>      node_type;    //!< alias
                typedef POINT<T>           point_type;   //!< alias
                typedef vector<node_type>  vector_type;  //!< alias

                inline explicit points(const size_t n=0) :
                vector_type(n,as_capacity) {}               //!< setup
                inline virtual ~points() throw()         {} //!< cleanup

                //! append a node by position
                inline points & operator<<( const point_type p) {const node_type _(p); this->push_back(_); return *this; }

                //! append a node with 2 coordinates
                inline points & add(const_type x, const_type y) { const point_type p(x,y); return (*this) << p; }

                //! append a node with 3 coordinates
                inline points & add(const_type x, const_type y, const type z) { const point_type p(x,y,z); return (*this) << p; }

                //! void compute metrics
                void update( const boundaries bnd )
                {
                    switch( this->size() )
                    {
                        case 0:  return;
                        case 1:  this->front().reset();        break;
                        case 2:  compute_two( bnd );  break;
                        default: compute_all(bnd);    break;
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);

                //! special 2 points cases
                inline void compute_two( const boundaries bnd )
                {
                    node_type       &nA = this->front(); nA.reset();
                    node_type       &nB = this->back();  nB.reset();
                    const point_type AB(nA.r,nB.r);
                    const_type       AB2 = AB.norm2();
                    if(AB2<=0) throw libc::exception(EDOM,"null norm");
                    const point_type t   = AB/sqrt_of(AB2);
                    switch(bnd)
                    {
                        case standard: nA.t = t; nB.t =  t; break;
                        case periodic: nA.t = t; nB.t = -t; break;
                    }

                }

                inline void compute_all(const boundaries bnd)
                {
                    vector_type &self = *this;
                    const size_t n    = self.size(); assert(n>=3);
                    const size_t nm1  = n-1;

                    // at boundaries
                    switch(bnd)
                    {
                        case standard:
                            break;

                        case periodic:
                            bulk_tangent(self[n],self[1],self[2]);
                            bulk_tangent(self[nm1],self[n],self[1]);
                            break;
                    }

                    // in bulk
                    for(size_t i=nm1;i>1;--i)
                    {
                        bulk_tangent(self[i-1],self[i],self[i+1]);
                    }
                }

                static inline void bulk_tangent( const node_type &nA , node_type &nB,  const node_type &nC )
                {
                    const point_type A = nA.r;
                    const point_type B = nB.r;
                    const point_type C = nC.r;
                    const point_type AC(A,C);
                    const_type       AC2 = AC.norm2(); if(AC2<=0) throw libc::exception(EDOM,"null norm");
                    const_type       ac  = sqrt_of(AC2);
                    nB.t = AC/ac;
                    const point_type ApCm2B = A+C-(B+B);
                    const type       num    = 4 * point_type::det(AC,ApCm2B);
                    nB.C = num/(ac*ac*ac);
                }

            };


        };

    }
}

#endif

