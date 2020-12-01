
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
            typedef key_address<2> NodeKey; //!< key for Nodes


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
                Y_DECL_ARGS(T,type);                             //!< aliases
                typedef Point<T,VTX>                PointType;   //!< alias
                typedef typename PointType::vertex  vertex;      //!< alias
                typedef typename PointType::Pointer SharedPoint; //!< alias
                typedef intr_ptr<NodeKey,Node>      Pointer;     //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Node( const SharedPoint &ptr ) throw() :
                Object(), SharedPoint(ptr), uuid(*this,*ptr),
                V(), A()
                {
                    
                }

                //! cleanup
                inline virtual ~Node() throw()
                {

                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! key for database of nodes
                inline const NodeKey & key() const throw() { return uuid; }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const NodeKey uuid;  //!< node+point
                const vertex  V;     //!< local velocity
                const vertex  A;     //!< local acceleration
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        }

    }

}

#endif
