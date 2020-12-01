
//! \file
#ifndef Y_GEOMETRIC_NODE_INCLUDED
#define Y_GEOMETRIC_NODE_INCLUDED 1

#include "y/mkl/geometric/point.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef key_address<2> NodeKey;


            //__________________________________________________________________
            //
            //! a unique node is a shared point with attribute
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Node : public Object, public Point<T,VTX>::Pointer
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);
                typedef Point<T,VTX>                PointType;
                typedef typename PointType::vertex  vertex;
                typedef typename PointType::Pointer SharedPoint;
                typedef intr_ptr<NodeKey,Node>      Pointer;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Node( const SharedPoint &ptr ) throw() :
                Object(), SharedPoint(ptr), uuid(*this,*ptr),
                V()
                {
                    
                }

                // cleanup
                inline virtual ~Node() throw()
                {

                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                // inline
                inline const NodeKey & key() const throw() { return uuid; }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const NodeKey uuid;
                const vertex  V;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif
