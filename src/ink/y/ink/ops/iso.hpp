//! \file
#ifndef Y_INK_OPS_ISO_INCLUDED
#define Y_INK_OPS_ISO_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/geometry/iso2d.hpp"

namespace upsylon
{
    namespace Ink
    {

        class Iso : public Area
        {
        public:
            //! prepare data for a given area
            explicit Iso( const Area &area );
            virtual ~Iso() throw();

            template <typename PIXMAP>
            void scan( const PIXMAP &pxm, const array<double> &z)
            {

                geometry::Iso2d::Scan(db,pxm,
                                      lower.x,upper.x,
                                      lower.y,upper.y,
                                      X,Y,
                                      z);
                geometry::Iso2d::convert(levels,db);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Iso);
            vector<double>          xy;
            const double          * X;
            const double          * Y;
            geometry::Iso2d::Levels db;

        public:
            geometry::Iso2d::LevelSet levels;
        };
    }
}

#endif

