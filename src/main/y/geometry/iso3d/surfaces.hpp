//! \file
#ifndef Y_GEOMETRY_ISO3D_SURFACES_INCLUDED
#define Y_GEOMETRY_ISO3D_SURFACES_INCLUDED 1

#include "y/geometry/iso3d/facets.hpp"
#include "y/geometry/contour.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! a surface is a database of points
            class Surface_ : public counted, public Points
            {
            public:
                const size_t index; //!< current level index

                explicit Surface_(const size_t) throw(); //!< setup with index
                virtual ~Surface_() throw();             //!< cleanup
                const size_t & key() const throw();      //!< key=index

                //! create/query single point
                Point_ * single(const Coordinate &,
                                const Vertex     &);

                //! create/query from a couple of points, da*db<0
                Point_ * couple(const Coordinate &ca,
                                const Vertex     &va,
                                const double      da,
                                const Coordinate &cb,
                                const Vertex     &vb,
                                const double      db);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Surface_);
            };

            //! dynamic surface
            typedef intr_ptr<size_t,Surface_> Surface;

            //! base type for surfaces
            typedef set<size_t,Surface,key_dumper,Contour::Allocator> SurfacesType;

            //! database of surfaces, key=level index
            class Surfaces : public SurfacesType
            {
            public:
                explicit Surfaces() throw(); //!< setup
                virtual ~Surfaces() throw(); //!< cleanup

                void create(size_t n);       //!< create [1:n]

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Surfaces);
            };




        }

    }
}

#endif

