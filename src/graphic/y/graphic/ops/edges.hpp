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
            explicit Edges( const size_t W, const size_t H);

            Pixmap<float>  g; //!< evaluated gradient
            Pixmap<Vertex> G; //!< normalised gradient
            Pixmap<float>  L; //!< local maxima

            void keepLocalMaxima( const Tile &tile ) throw();
            void keepLocalMaxima( Tiles      &tiles );
          

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
        };

    }

}

#endif


