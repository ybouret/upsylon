//! \file

#ifndef Y_GRAPHIC_OPS_EDGES_INCLUDED
#define Y_GRAPHIC_OPS_EDGES_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"
#include "y/graphic/ops/histogram.hpp"
#include "y/graphic/linked.hpp"

namespace upsylon {

    namespace Graphic {

        class Edge : public Object, public PList, public core::inode<Edge>
        {
        public:
            typedef core::list_of_cpp<Edge> List;
            const size_t label;
            const bool   hard;

            explicit Edge(const size_t edgeLabel) throw();
            virtual ~Edge() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

        //! Edges detection algorithms
        class Edges : public Pixmap<size_t>, public Edge::List
        {
        public:
            static const uint8_t HARD = 255; //!< hard edge value
            static const uint8_t SOFT = 128; //!< soft edge value
            static const uint8_t NONE =   0; //!< no edge

            virtual ~Edges() throw();                        //!< cleanup
            explicit Edges( const size_t W, const size_t H); //!< setup

            float           gmax;          //!< global max gradient
            Pixmap<float>   g;             //!< evaluated gradient
            Pixmap<Vertex>  G;             //!< normalised gradient
            Pixmap<uint8_t> L;             //!< local maxima indicator
            Histogram       hist;          //!< local maxima histogram
            uint8_t         hardThreshold; //!< hard threshold from histogram
            uint8_t         softThreshold; //!< soft threshold from histogram


            void keepLocalMaxima( Tiles &tiles ); //!< gradient+direction    => localMaxima+histogram
            void applyThresholds( Tiles &tiles ); //!< histogram+localMaxima => update localMaxima to hard/soft

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            void keepLocalMaxima( Tile  &tile  ) throw();
            void applyThresholds( Tile &tile   ) throw();

        };

    }

}

#endif


