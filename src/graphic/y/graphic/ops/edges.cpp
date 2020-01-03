
#include "y/graphic/ops/edges.hpp"

namespace upsylon {

    namespace Graphic {

        Edge:: Edge(const size_t edgeLabel) throw() :
        label(edgeLabel),
        flag(Feeble)
        {
        }

        Edge:: ~Edge() throw()
        {
        }

        
        Edges:: ~Edges() throw()
        {
        }

        Edges:: Edges( const size_t W, const size_t H) :
        Pixmap<size_t>(W,H),
        gmax(0),
        data(W,H),
        g(W,H),
        G(W,H),
        L(W,H),
        P(W,H),
        hist(),
        strongThreshold(0),
        feebleThreshold(0)
        {
        }


        void   Edges:: computeGradients(const Gradients::Pointer &gradients,
                                        Tiles                    &tiles)
        {
            gradients->run(g, G, data, tiles, gmax);
        }

        void Edges:: processData(const Gradients::Pointer &gradients,
                                 Tiles                    &tiles)
        {

            computeGradients(gradients,tiles);
            keepLocalMaxima(tiles);
            const size_t np = applyThresholds(tiles);
            build(np);
        }

    }
}
