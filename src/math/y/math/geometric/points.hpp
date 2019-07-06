//! \file
#ifndef Y_MATH_GEOM_POINTS_INCLUDED
#define Y_MATH_GEOM_POINTS_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Geometric
        {

            //! generic boundaries for operations
            enum Boundaries
            {
                Standard, //!< for standard curve
                Periodic  //!< for periodic curve
            };

            //! throw exception if value<=0
            template <typename T> T    CheckNorm2(const T value, const char *ctx);
            
            //! save float as generic value
            template <typename T> void SaveReal(ios::ostream &fp, const T value);

            //! contains dedicated types
            template <const size_t DIM,typename T> struct InfoFor;

            //! handle point[2|3]d<T> and POINT<T>
            template <typename T,template <typename> class POINT>
            struct PointInfoFor
            {
                Y_DECL_ARGS(T,type);                                                           //!< alias
                typedef POINT<type>                     Type;                                  //!< user point type
                static const size_t                     Dimension = sizeof(Type)/sizeof(type); //!< the point dimension
                typedef InfoFor<Dimension,mutable_type> InfoType;                              //!< info for this topolgy
                typedef typename InfoType::PointType    Core;                                  //!< internal point[2|3]d<T>
                typedef typename InfoType::Angle        Angle;                                 //!< alias for angles
                
                static inline Core       & Type2Core( Type       &t ) throw() { return *(Core *)&t;       } //!< POINT -> point[2|3]
                static inline const Core & Type2Core( const Type &t ) throw() { return *(const Core *)&t; } //!< POINT -> point[2|3], const
                static inline Type &       Core2Type( Core       &c ) throw() { return *(Type *)&c;       } //!< point[2|3] -> POINT
                static inline const Type & Core2Type( const Core &c ) throw() { return *(const Type *)&c; } //!< point[2|3] -> POINT, const
                static inline Core         Origin()                   throw() { return Core();            } //!< [(0,0)|(0,0,0)]

                //! save Dimension reals
                static inline void SaveReals( ios::ostream &fp, const_type *r )
                {
                    assert(r!=NULL);
                    SaveReal(fp,r[0]);
                    for(size_t i=1;i<Dimension;++i)
                    {
                        fp << ' '; SaveReal(fp,r[i]);
                    }
                }

                //! save Type
                static inline ios::ostream & SaveType( ios::ostream &fp, const Type &t ) { SaveReals(fp,(const_type *)&t); return fp; }

                //! save Core
                static inline ios::ostream & SaveCore( ios::ostream &fp, const Core &c ) { SaveReals(fp,(const_type *)&c); return fp; }

            };

            //! 2D types
            template <typename T> struct InfoFor<2,T>
            {
                typedef point2d<T> PointType; //!< core point type
                //! angles for 2D
                class Angle
                {
                public:
                    T theta; //!< polar angle
                    inline  Angle( T t=0 ) throw() : theta(t) {}              //!< setup
                    inline ~Angle() throw() { theta=0; }                      //!< cleanup
                    inline  Angle(const Angle &a) throw() : theta(a.theta) {} //!< copy
                    inline  Angle & operator=( const Angle &a ) throw()       //|
                    {                                                         //|
                        theta = a.theta;                                      //|
                        return *this;                                         //|
                    }                                                         //!< assign

                    //! compute difference of angle
                    inline void find( const PointType &u, const PointType &v) throw()
                    {
                        const T c = u.x*v.x + u.y*v.y;
                        const T s = u.x*v.y - u.y*v.x;
                        theta = acos_of(c);
                        if(s<0) theta=numeric<T>::two_pi-theta;
                    }

                    //! rotate by a fraction of angle
                    inline PointType rotate(const PointType &u, const T factor) const throw()
                    {
                        const T tt = theta*factor;
                        const T c  = cos_of(tt);
                        const T s  = sin_of(tt);
                        return PointType( u.x*c-u.y*s, u.x*s + u.y*c);
                    }
                };
            };

            //! 3D type
            template <typename T> struct InfoFor<3,T>
            {
                typedef point3d<T> PointType; //!< core point type

                //! angles in 3d
                class Angle
                {
                public:
                    T theta; //!< polar angle
                    T phi;   //!< azimutal angle
                    inline  Angle( T t=0, T p=0) throw() : theta(t), phi(p) {}            //!< setup
                    inline ~Angle() throw() {theta=phi=0;}                                //!< cleanup
                    inline  Angle(const Angle &a) throw() : theta(a.theta), phi(a.phi) {} //!< copy
                    inline  Angle & operator=( const Angle &a ) throw()                   //|
                    {                                                                     //|
                        theta = a.theta;                                                  //|
                        phi   = a.phi;                                                    //|
                        return *this;                                                     //|
                    }                                                                     //!< assign

                    //! compute angles
                    inline void find( const PointType &, const PointType &) throw()
                    {
                    }

                    //! rotate by a fraction of angles
                    inline PointType rotate(const PointType &u, const T ) const throw()
                    {
                        return u;
                    }
                };
            };
            
            //! utility collection of points
            template <
            typename                  T,
            template <typename> class POINT,
            template <typename> class SEQUENCE = vector_for<memory::global>::device >
            class Points : public SEQUENCE< POINT<T> >
            {
            public:
                Y_DECL_ARGS(T,type);                                            //!< aliases
                typedef PointInfoFor<T,POINT>                 PointInfo;        //!< alias for point
                typedef typename PointInfo::Type              PointType;        //!< POINT<T>
                typedef typename PointInfo::Core              CorePoint;        //!< point[2|3]d<T>
                typedef SEQUENCE<PointType>                   SequenceType;     //!< alias for SEQUENCE
                typedef typename SequenceType::iterator       iterator;         //!< iterator alias
                typedef typename SequenceType::const_iterator const_iterator;   //!< const iterator alias

                inline explicit Points() {}                                                            //!< setup
                inline explicit Points(const size_t n, const as_capacity_t &_) : SequenceType(n,_) {}  //!< setup with capacity
                inline virtual ~Points() throw() {}                                                    //!< cleanup
                inline Points( const Points &other ) : SequenceType(other) {}                          //!< copy

                //! add a point with two coordinates
                inline Points & add(param_type x, param_type y)               { const PointType p(x,y);   this->push_back(p); return *this; }

                //! add a point with three coordinates
                inline Points & add(param_type x, param_type y, param_type z) { const PointType p(x,y,z); this->push_back(p); return *this; }
                
                
            private:
                Y_DISABLE_ASSIGN(Points);

            };
            
        }
    }
}

#endif

