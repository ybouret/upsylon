//! \file
#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{

    //! curve for 2D/3D points
    struct curve
    {
        //! internal node
        template <typename T, template <class> class POINT>
        class node
        {
        public:
            typedef POINT<T> point_type; //!< internal point type

            point_type r; //!< position
            point_type t; //!< tangent
            point_type n; //!< normal

            inline  node() throw() : r(), t(), n() {}                       //!< setup, all zero
            inline ~node() throw() {}                                       //!< cleanup
            inline  node(const node &o) throw() : r(o.r), t(o.t), n(o.n) {} //!< copy

        private:
            Y_DISABLE_ASSIGN(node);
        };

        //! array of points
        template <typename T, template <class> class POINT>
        class points : public vector< node<T,POINT> >
        {
        public:
            typedef node<T,POINT>      node_type;    //!< alias
            typedef POINT<T>           point_type;   //!< alias
            typedef vector<point_type> vector_type;  //!< alias
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(points);
        };


    };


}

#endif

