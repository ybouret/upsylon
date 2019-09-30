//! \file
#ifndef Y_GEOM_ISO2D_LEVELS_INCLUDED
#define Y_GEOM_ISO2D_LEVELS_INCLUDED 1

#include "y/geometry/iso2d/segments.hpp"
#include "y/associative/set.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //__________________________________________________________________
            //
            //! base class for a level
            //__________________________________________________________________
            typedef set<Edge,Point,Edge::Hasher,memory::global> Points;

            struct Scanner; //!< forward declaration

            //__________________________________________________________________
            //
            //! a level is a set of points, has segments and optional curves
            //__________________________________________________________________
            class Level_ : public counted, public Points
            {
            public:
                const size_t     index; //!< index in the contour::level
                Segments         slist; //!< list of associated segments
                Lines            iso;   //!< sequence of isolines

                explicit         Level_(const size_t ) throw();    //!< setup
                virtual         ~Level_() throw();                 //!< cleanup
                const size_t   & key() const throw();              //!< key for LevelSet
                void             buildLines(const bool keep);      //!< compile to isolines

                //! check consistency
                void check() const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Level_);
                friend struct Scanner;

                //! create/query a single point
                Point_ *single(const Coordinate &c, const Vertex &v );


                //! create/query a point on the edge(ca,cb)
                Point_ *couple(const Coordinate &ca, const Vertex &va, const double da,
                               const Coordinate &cb, const Vertex &vb, const double db);

                //! make segment from two single points
                void    full(const Coordinate &ca, const Vertex &va,
                             const Coordinate &cb, const Vertex &vb);

                //! make segment from zero vertex and intersection, da*db<0
                void    inter1(const Coordinate &cz, const Vertex &vz,
                               const Coordinate &ca, const Vertex &va, const double da,
                               const Coordinate &cb, const Vertex &vb, const double db);

                //! make segment from two intersections: ds*da<0, ds*db<0,
                void    inter2(const Coordinate &cs, const Vertex &vs, const double ds,
                               const Coordinate &ca, const Vertex &va, const double da,
                               const Coordinate &cb, const Vertex &vb, const double db);
            };

            //__________________________________________________________________
            //
            //! dynamic level
            //__________________________________________________________________
            typedef intr_ptr<size_t,Level_>                         Level;

            //__________________________________________________________________
            //
            //! base class for level_set
            //__________________________________________________________________
            typedef set<size_t,Level,key_dumper,Contour::Allocator> LevelSetType;

            //__________________________________________________________________
            //
            //! a database of levels
            //__________________________________________________________________
            class LevelSet : public LevelSetType
            {
            public:
                explicit LevelSet() throw();    //!< setup
                virtual ~LevelSet() throw();    //!< cleanup
                void     create(const size_t n); //!< create [1:n] levels
                void     check() const;          //!< check all levels
                void     buildLines(const bool keep); //!< build isolines for all levels

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LevelSet);
            };


        }
    }
}

#endif
