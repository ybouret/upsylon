//! \file

#ifndef Y_GRAPHIC_OPS_EDGES_INCLUDED
#define Y_GRAPHIC_OPS_EDGES_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"
#include "y/graphic/ops/histogram.hpp"

namespace upsylon {

    namespace Graphic {

        //! Edges detection algorithms
        class Edges
        {
        public:
            static const uint8_t HARD = 255; //!< hard edge value
            static const uint8_t SOFT = 128; //!< soft edge value

            virtual ~Edges() throw();                        //!< cleanup
            explicit Edges( const size_t W, const size_t H); //!< setup

            float           gmax;          //!< global max gradient
            Pixmap<float>   g;             //!< evaluated gradient
            Pixmap<Vertex>  G;             //!< normalised gradient
            Pixmap<uint8_t> L;             //!< local maxima indicator
            Histogram       hist;          //!< local maxima histogram
            uint8_t         hardThreshold; //!< hard threshold from histogram
            uint8_t         softThreshold; //!< soft threshold from histogram
            
            void keepLocalMaxima( Tiles &tiles ); //!< gradient+direction => localMaxima+histogram
            void applyThresholds( Tiles &tiles ); //!< histogram+localMaxim => update localMaxima to hard/soft

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            void keepLocalMaxima( Tile  &tile  ) throw();
            void applyThresholds( Tile &tile   ) throw();

        };

    }

}

#endif


