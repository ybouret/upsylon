//! \file

#ifndef Y_GRAPHIC_OPS_EDGES_INCLUDED
#define Y_GRAPHIC_OPS_EDGES_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"
#include "y/graphic/ops/histogram.hpp"

namespace upsylon {

    namespace Graphic {

        class Edges
        {
        public:
            virtual ~Edges() throw();
            explicit Edges( const size_t W, const size_t H);

            float           gmax;
            Pixmap<float>   g; //!< evaluated gradient
            Pixmap<Vertex>  G; //!< normalised gradient
            Pixmap<uint8_t> L; //!< local maxima
            Histogram       hist; //!< local maxima histogram

            void keepLocalMaxima( Tile  &tile  ) throw();
            void keepLocalMaxima( Tiles &tiles );

            void threshold( Tile &tile ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
        };

    }

}

#endif


