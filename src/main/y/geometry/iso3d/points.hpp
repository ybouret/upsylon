
//! \file
#ifndef Y_GEOMETRY_ISO3D_POINTS_INCLUDED
#define Y_GEOMETRY_ISO3D_POINTS_INCLUDED 1

#include "y/geometry/iso3d/edge.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon {
    
    namespace geometry {
        
        namespace Iso3D {
            
            //==================================================================
            //
            //! base class for unique point
            //
            //==================================================================
            class Point_ : public counted_object
            {
            public:
                const Edge     location; //!< logical
                const Vertex   position; //!< physical
                
                explicit Point_( const Edge &, const Vertex &) throw(); //!< setup
                virtual ~Point_() throw();                              //!< cleanup
                
                const  Edge & key() const throw(); //!< key fo r database
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point_);
            };
            
            
            //==================================================================
            //
            //! dynamic unique point
            //
            //==================================================================
            typedef intr_ptr<Edge,Point_> Point;
            
            
            //==================================================================
            //
            //! base class for database of unique points
            //
            //==================================================================
            typedef set<Edge,Point,Edge::Hasher,memory::global> PointsType;
            
            
            //==================================================================
            //
            //! database of unique points
            //
            //==================================================================
            class Points : public PointsType
            {
            public:
                explicit Points() throw(); //!< setup
                virtual ~Points() throw(); //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Points);
            };
            
            
        }
        
    }
    
}

#endif

