//! \file
#ifndef Y_EUCLIDEAN_POINT_INCLUDED
#define Y_EUCLIDEAN_POINT_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/type/args.hpp"
#include "y/type/bzset.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/intr.hpp"
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
                inline explicit Point() throw() : position(), uuid(*this) {}                //!< setup with default position
                inline explicit Point(const Vertex p) throw() : position(p), uuid(*this) {} //!< setup
                inline virtual ~Point() throw() { bzset(position); }                        //!< cleanup
                inline const PointKey & key() const throw() { return uuid; }                //!< for database

                //! output by mapping to point<>
                static inline ios::ostream & Print( ios::ostream &fp, const Vertex &v)
                {
                    return Core::Print(fp,aliasing::cast<VTX,Vertex>(v));
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
static const size_t Dimensions =     PointType::Dimensions; \
typedef typename PointType::Pointer  SharedPoint;  \
typedef typename PointType::Vertex   Vertex;      \
typedef typename PointType::VTX      VTX

            typedef key_address<2> NodeKey;


            template <typename T,template <class> class POINT>
            class Node : public Object
            {
            public:
                Y_EUCLIDEAN_POINT_ARGS();
                typedef intr_ptr<NodeKey,Node> Pointer;

                const SharedPoint point;
                const Vertex      celerity;
                const_type        speed;
                const Vertex      uT;
                const Vertex      uN;
                const Vertex      Q;
                const Vertex      W;
                const NodeKey     uuid;

                inline explicit Node( const SharedPoint &p ) throw() :
                point(p),
                celerity(),
                speed(0),
                uT(),
                uN(),
                Q(),
                W(),
                uuid(*p,*this)
                {
                }

                inline virtual ~Node() throw()
                {
                    setFixed();
                }

                inline const NodeKey & key() const throw() { return uuid; }

                inline void setCelerity( const Vertex v ) throw()
                {
                    setFixed();
                    const VTX  &V = aliasing::cast<VTX,Vertex>(v);
                    const_type v2 = V.norm2();
                    if(v2>0)
                    {
                        aliasing::_(celerity) = v;
                        aliasing::_(speed)    = sqrt_of(v2);
                        aliasing::_(uT)       = celerity/speed;
                    }
                }

                inline void setFixed() throw()
                {
                    bzset_(celerity);
                    bzset_(speed);
                    bzset_(uT);
                    bzset_(uN);
                    bzset_(Q);
                    bzset_(W);
                }
                
                inline Vertex compute(const_type u) const
                {
                    //return point->position + u*celerity + u*u*Q + u*u*u*W;
                    return point->position + u*(celerity + u*(Q + u*W));

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
                
                
            };

#define Y_EUCLIDEAN_NODE_ARGS()                \
typedef Node<T,POINT>              NodeType;   \
typedef typename NodeType::Pointer SharedNode
            
        }

    }

}

#endif

