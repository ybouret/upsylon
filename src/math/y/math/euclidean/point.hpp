//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/intr.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"
#include "y/container/key-address.hpp"
#include "y/associative/set.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    
    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! helper to find the right point[2|3]d<T>
            //
            //==================================================================
            template <typename T,size_t> struct __Core;

            //==================================================================
            //
            //! map to point2d<T>
            //
            //==================================================================
            template <typename T> struct __Core<T,2>
            {
                typedef point2d<T> Type; //!< alias
                static inline ios::ostream &Print(ios::ostream &fp, const Type &v)
                {
                    fp("%.15g %.15g", v.x, v.y);
                    return fp;
                }
            };

            //==================================================================
            //
            //! map to point3d<T>
            //
            //==================================================================
            template <typename T> struct __Core<T,3>
            {
                typedef point3d<T> Type; //!< alias
                static inline ios::ostream &Print(ios::ostream &fp, const Type &v)
                {
                    fp("%.15g %.15g %.15g", v.x, v.y, v.z);
                    return fp;
                }
            };

            //==================================================================
            //
            //! base class for object
            //
            //==================================================================
            typedef counted_object Object;

            //==================================================================
            //
            //! key for unique allocated point
            //
            //==================================================================
            typedef key_address<1> PointKey;

            //==================================================================
            //
            //
            //! a point has a position
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT >
            class Point : public Object
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_DECL_ARGS(T,type);                                          //!< alias
                typedef POINT<type>                           Vertex;         //!< the data handling vertex
                static const size_t Dimensions = sizeof(Vertex)/sizeof(type); //!< dimensions
                typedef  __Core<type,Dimensions>              Core;           //!< alias
                typedef typename Core::Type                   VTX;            //!< mapping point type
                typedef intr_ptr<PointKey,Point>              Pointer;        //!< for shared point
                typedef set<PointKey,Pointer>                 DataBase;       //!< alias

                //==============================================================
                //
                // members
                //
                //==============================================================
                Vertex         position; //!< current position
                const PointKey uuid;     //!< uuid based on address

                //==============================================================
                //
                // methods
                //
                //==============================================================
                inline explicit Point() throw() : position(), uuid(*this) {}  //!< setup with default position
                inline explicit Point(const Vertex p) throw() : position(p){} //!< setup
                inline virtual ~Point() throw() { bzset(position); }          //!< cleanup
                inline const PointKey & key() const throw() { return uuid; }  //!< for database

                //! output by mapping to point<>
                static inline ios::ostream & Print( ios::ostream &fp, const Vertex &v)
                {
                    const VTX &V = aliasing::cast<VTX,Vertex>(v);
                    return Core::Print(fp,V);
                }

                inline VTX       & operator*()       throw() { return aliasing::cast<VTX,Vertex>(position); }
                inline const VTX & operator*() const throw() { return aliasing::cast<VTX,Vertex>(position); }

                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point);
            };

            //! forwarding type
#define Y_EUCLIDEAN_POINT_ARGS()                  \
Y_DECL_ARGS(T,type);                              \
typedef Point<T,POINT>               PointType;   \
typedef typename PointType::Pointer  SharedPoint; \
typedef typename PointType::DataBase Points;      \
typedef typename PointType::Vertex   Vertex;      \
typedef typename PointType::VTX      VTX

            //==================================================================
            //
            //
            //! a dynamic node, which is a shared point with local data
            //
            //
            //==================================================================
            template <typename T,template <class> class POINT >
            class PointNode : public Point<T,POINT>::Pointer, public core::inode< PointNode<T,POINT> >
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_POINT_ARGS();                        //!< aliases
                typedef core::inode< PointNode<T,POINT> > iNode; //!< alias
                typedef core::list_of_cpp<PointNode>      List;  //!< base type for list


                //==============================================================
                //
                // members
                //
                //==============================================================
                const Vertex celerity; //!< celerity based on position
                const_type   speed;    //!< |celerity|
                const Vertex tangent;  //!< celerity/speed

                inline void setCelerity(const Vertex v) throw()
                {
                    (Vertex      &)celerity  = v;
                    const_type     v2        = ( (const VTX&)celerity ).norm2();
                    if(v2<=0)
                    {
                        bzset_(speed);
                        bzset_(tangent);
                    }
                    else
                    {
                        (mutable_type&)speed   = sqrt_of(v2);
                        (Vertex&)      tangent = celerity/speed;
                    }
                }

                
                //==============================================================
                //
                // methods
                //
                //==============================================================
                //! setup
                inline explicit PointNode( const SharedPoint &P ) throw() :
                SharedPoint(P), iNode(), celerity(), speed(0) {}
                //! cleanup
                inline virtual ~PointNode() throw() {}
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(PointNode);
            };

#define Y_EUCLIDEAN_POINTNODE_ARGS()      \
typedef PointNode<T,POINT>      NodeType; \
typedef typename NodeType::List NodeList
            
        }

    }

}

#endif

