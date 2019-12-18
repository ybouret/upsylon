//! \file

#ifndef Y_GRAPHIC_OPS_EDGES_INCLUDED
#define Y_GRAPHIC_OPS_EDGES_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"

namespace upsylon {

    namespace Graphic {

        class Edges
        {
        public:
            virtual ~Edges() throw();

            Pixmap<float>  g; //!< evaluated gradient
            Pixmap<Vertex> G; //!< normalised gradient
            Pixmap<float>  L; //!< local maxima

            void keepLocalMaxima( const Tile &tile )
            {
                const Point up = tile.upper;
                const Point lo = tile.lower;
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
        };

    }

}

#endif


