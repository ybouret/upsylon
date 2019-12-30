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
            static const uint8_t HARD = 255;
            static const uint8_t SOFT = 128;
            virtual ~Edges() throw();
            explicit Edges( const size_t W, const size_t H);

            float           gmax;
            Pixmap<float>   g; //!< evaluated gradient
            Pixmap<Vertex>  G; //!< normalised gradient
            Pixmap<uint8_t> L; //!< local maxima
            Histogram       hist; //!< local maxima histogram
            uint8_t         hardThreshold;
            uint8_t         softThreshold;
            
            void keepLocalMaxima( Tile  &tile  ) throw();
            void keepLocalMaxima( Tiles &tiles );

            void applyThresholds( Tile &tile   ) throw();
            void applyThresholds( Tiles &tiles );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
        };

    }

}

#endif


