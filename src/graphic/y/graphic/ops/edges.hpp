//! \file

#ifndef Y_GRAPHIC_OPS_EDGES_INCLUDED
#define Y_GRAPHIC_OPS_EDGES_INCLUDED 1

#include "y/graphic/ops/filter/gradients.hpp"
#include "y/graphic/ops/blur.hpp"
#include "y/graphic/ops/histogram.hpp"
#include "y/graphic/linked.hpp"
#include "y/graphic/parallel/ops.hpp"
#include "y/core/inode.hpp"

namespace upsylon {

    namespace Graphic {

        //! an edge is a list of points
        class Edge : public Object, public PList, public inode<Edge>
        {
        public:
            //! Edge force
            enum Flag
            {
                Strong, //!< a strong edge
                Feeble  //!< a feeble edge
            };

            typedef core::list_of_cpp<Edge> List; //!< alias
            const size_t label; //!< edge label
            const Flag   flag;  //!< edge flag, removed if remains feeble

            explicit Edge(const size_t edgeLabel) throw(); //!< setup as feeble
            virtual ~Edge() throw();                       //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

        //! Edges detection algorithms
        class Edges : public Indices, public Edge::List
        {
        public:
            static const uint8_t STRONG = 255; //!< hard edge value
            static const uint8_t FEEBLE = 128; //!< soft edge value
            static const uint8_t NoEDGE =   0; //!< no edge

            //! pointer to local points
            struct LocalMaxima
            {
                Point *points; //!< points from P
                size_t count;  //!< local count
            };

            static const size_t  LocalHistogramBytes = Histogram::BINS * sizeof(size_t); //!< alias
            static const size_t  LocalMaximaBytes    = sizeof(LocalMaxima);              //!< alias

            virtual ~Edges() throw();                        //!< cleanup
            explicit Edges( const size_t W, const size_t H); //!< setup

            float           gmax;            //!< global max gradient
            Pixmap<float>   data;            //!< some intensity data
            Pixmap<float>   g;               //!< evaluated gradient
            Pixmap<Vertex>  G;               //!< normalised gradient
            Pixmap<uint8_t> L;               //!< local maxima indicator
            Pixmap<Point>   P;               //!< memory for edges building
            Histogram       hist;            //!< local maxima histogram
            uint8_t         strongThreshold; //!< strong threshold from histogram
            uint8_t         feebleThreshold; //!< feeble threshold from histogram

            //! load data from source
            template <typename T>
            inline void load( const Pixmap<T> &source, const Blur *blur, Tiles &tiles)
            {
                if(blur)
                {
                    Pixmap<float> &temp = g;
                    Ops::Convert(tiles, temp, source);
                    blur->apply(data,temp,tiles);
                }
                else
                {
                    Ops::Convert(tiles,data,source);
                }
            }

            //! compute g and G from data
            void   computeGradients(const Gradients::Pointer &gradients,
                                    Tiles                    &tiles);

            //! take gradient and direction to keep local maxima, and make an histogram of strength
            void   keepLocalMaxima( Tiles &tiles );

            //! compute the Hard/Soft thresholds and binarize local maxima
            /**
             keep track of non-zero edges points, compacted in P
             */
            size_t applyThresholds( Tiles &tiles );

            //! build edges from recorded Strong/Feeble points, delete feeble edges
            void build(size_t np);

            //! integrated procedure
            template <typename T>
            inline void find(const Pixmap<T>          &source,
                             const Blur               *blur,
                             const Gradients::Pointer &gradients,
                             Tiles                    &tiles)
            {
                load(source,blur,tiles);
                processData(gradients,tiles);
            }

            //! transfert only edges components
            template <typename T>
            inline void transfer(Pixmap<T>       &target,
                                 const Pixmap<T> &source) const
            {
                for( const Edge *edge=head;edge;edge=edge->next)
                {
                    Linked::Transfer(target, source, *edge);
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            void keepLocalMaxima( Tile &tile  ) throw();
            void applyThresholds( Tile &tile  ) throw();
            void processData(const Gradients::Pointer &gradients,
                             Tiles                    &tiles);
        };

    }

}

#endif


