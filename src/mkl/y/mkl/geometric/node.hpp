
//! \file
#ifndef Y_GEOMETRIC_NODE_INCLUDED
#define Y_GEOMETRIC_NODE_INCLUDED 1

#include "y/mkl/geometric/point.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! a node is a shared point with attribute
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Node : public Object, public Point<T,VTX>::Pointer
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                             //!< aliases
                typedef Point<T,VTX>                PointType;   //!< alias
                typedef typename PointType::vertex  vertex;      //!< alias
                typedef typename PointType::Pointer SharedPoint; //!< alias
                typedef arc_ptr<Node>               Pointer;     //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Node( const SharedPoint &ptr ) throw() :
                Object(), SharedPoint(ptr), A()
                {
                    
                }

                //! cleanup
                inline virtual ~Node() throw()
                {

                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                vertex A; //!< the second derivative
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif
