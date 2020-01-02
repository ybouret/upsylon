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
        class Edges : public Indices, public Edge::List
        {
        public:
            static const uint8_t HARD = 255; //!< hard edge value
            static const uint8_t SOFT = 128; //!< soft edge value
            static const uint8_t NONE =   0; //!< no edge

            struct LocalMaxima
            {
                Point *points;
                size_t count;
            };

            static const size_t  LocalHistogramBytes = Histogram::BINS * sizeof(size_t);
            static const size_t  LocalMaximaBytes    = sizeof(LocalMaxima);

            virtual ~Edges() throw();                        //!< cleanup
            explicit Edges( const size_t W, const size_t H); //!< setup

            float           gmax;          //!< global max gradient
            Pixmap<float>   g;             //!< evaluated gradient
            Pixmap<Vertex>  G;             //!< normalised gradient
            Pixmap<uint8_t> L;             //!< local maxima indicator
            Pixmap<Point>   P;             //!< memory for edges building
            Histogram       hist;          //!< local maxima histogram
            uint8_t         hardThreshold; //!< hard threshold from histogram
            uint8_t         softThreshold; //!< soft threshold from histogram

            //! take gradient and direction to keep local maxima, and make an histogram of strength
            void   keepLocalMaxima( Tiles &tiles );

            //! compute the Hard/Soft thresholds and binarize local maxima
            /**
             keep track of non-zero edges points, compacted in P
             */
            size_t applyThresholds( Tiles &tiles );

            //! build and merge edges
            void build( const size_t np );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            void keepLocalMaxima( Tile  &tile  ) throw();
            void applyThresholds( Tile &tile   ) throw();

        };

    }

}

#endif


