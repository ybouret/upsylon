//! \file
#ifndef Y_GEOMETRY_ISO3D_COORD_INCLUDED
#define Y_GEOMETRY_ISO3D_COORD_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace geometry {
        
        namespace Iso3D {

            //==================================================================
            //
            //! alias for physical position
            //
            //==================================================================
            typedef point3d<double> Vertex;

            //==================================================================
            //
            //! logical position
            //
            //==================================================================
            class Coordinate
            {
            public:
                const unit_t i; //!< dim0
                const unit_t j; //!< dim1
                const unit_t k; //!< dim2

                Coordinate(const unit_t,const unit_t,const unit_t) throw(); //!< setup
                ~Coordinate() throw();                                      //!< cleanup
                Coordinate(const Coordinate &) throw();                     //!< copy

                static int  Compare(const Coordinate &,const Coordinate &)     throw(); //!< lexicographic
                friend bool operator==(const Coordinate &, const Coordinate &) throw(); //!< component wise
                friend bool operator!=(const Coordinate &, const Coordinate &) throw(); //!< component wise
                friend bool operator<(const Coordinate &,const Coordinate &)   throw(); //!< lexicographic
                void        __run( hashing::function &H ) const throw();                //!< for Edge hashing

            private:
                Y_DISABLE_ASSIGN(Coordinate);
            };
        }
    }
}

#endif

