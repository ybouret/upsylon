//! \file

#ifndef Y_GRAPHIC_FILTER_GRADIENT_INCLUDED
#define Y_GRAPHIC_FILTER_GRADIENT_INCLUDED 1

#include "y/graphic/ops/filter.hpp"

namespace upsylon {

    namespace Graphic {

        //! compute gradients with two filters
        class Gradients : public Object
        {
        public:
            typedef Kernel::Filter::Weight  Weight;  //!< alias
            typedef Kernel::Filter::Weights Weights; //!< alias
            typedef arc_ptr<Gradients>      Pointer; //!< alias

            const Kernel::Filter::Pointer X; //!< gradient in X direction
            const Kernel::Filter::Pointer Y; //!< gradient in Y direction

            //! setup
            explicit Gradients(const Kernel::Filter::Pointer &GX,
                               const Kernel::Filter::Pointer &GY) throw();
            //! cleanup
            virtual ~Gradients() throw();

            //! copy
            Gradients(const Gradients &) throw();

            //! compute gradient and direction in a region, set global max
            template <typename T>
            inline void apply(Pixmap<float>   &g,
                              Pixmap<Vertex>  &G,
                              const Pixmap<T> &source,
                              float           &gmax,
                              const Point      lower,
                              const Point      upper) const throw()
            {
                // get X weights
                const Weights &WX = X->weights;
                const size_t   nx = WX.size();

                // get Y weights
                const Weights &WY = Y->weights;
                const size_t   ny = WY.size();

                // set local max gradient
                float          gm = 0.0f;

                // loop over coordinates
                for(unit_t y=upper.y;y>=lower.y;--y)
                {
                    Pixmap<float>::RowType            &gg = g[y];
                    Pixmap<Vertex>::RowType           &GG = G[y];
                    for(unit_t x=upper.x;x>=lower.x;--x)
                    {
                        const Point org(x,y);

                        // compute Gx around org
                        float       Gx = 0.0f;
                        for(size_t i=0;i<nx;++i)
                        {
                            const Weight &w = WX[i];
                            const Point   p = org + w.point;
                            Gx += w.value * static_cast<float>( source(p) );
                        }

                        // compute Gy around org
                        float       Gy = 0.0f;
                        for(size_t j=0;j<ny;++j)
                        {
                            const Weight &w = WY[j];
                            const Point   p = org + w.point;
                            Gy += w.value * static_cast<float>( source(p) );
                        }

                        // compute direction and gradient
                        {
                            Vertex &G = GG[x];
                            G.x = Gx; Gx *= Gx;
                            G.y = Gy; Gy *= Gy;
                            const float g2 = Gx+Gy;
                            if(g2>0.0f)
                            {
                                const float g = sqrtf(g2);
                                if(g>gm)
                                {
                                    gm = g;
                                }
                                gg[x] = g;
                                G.x  /= g;
                                G.y  /= g;
                            }
                            else
                            {
                                gg[x] = 0.0f;
                            }
                        }
                    }
                }
                gmax = gm;
            }

            //! compute in parallel
            template <typename T>
            void run(Pixmap<float>   &g,
                     Pixmap<Vertex>  &G,
                     const Pixmap<T> &source,
                     Tiles           &tiles,
                     float           &gmax) const
            {
                tiles.cacheAcquireFor<float>();

                struct Task {
                    const Gradients *self;
                    Pixmap<float>   *g;
                    Pixmap<Vertex>  *G;
                    const Pixmap<T> *source;
                    Tiles           *tiles;

                    static inline
                    void Run( void *args, parallel &ctx, lockable & ) throw()
                    {
                        Task &task = *static_cast<Task *>(args);
                        Tile &tile = (*task.tiles)[ctx.rank];
                        assert(tile.size>=sizeof(float));
                        task.self->apply( *task.g, *task.G, *task.source, tile.get<float>(), tile.lower, tile.upper );
                    }

                };

                Task task = { this, &g, &G, &source, &tiles };
                tiles.loop().run( Task::Run, &task);
                gmax = tiles.globalMax<float>();
            }


        private:
            Y_DISABLE_ASSIGN(Gradients);
        };

        //! helper to declare gradients
#define Y_GRAPHICS_GRADIENTS_DECL(ID) static Gradients * Gradients##ID()

        //! helper to implement gradients
#define Y_GRAPHICS_GRADIENTS_IMPL(CLASS,ID)     \
Gradients * CLASS :: Gradients##ID() {          \
const Kernel::Filter::Pointer GX = new X##ID(); \
const Kernel::Filter::Pointer GY = new Y##ID(); \
return new Gradients(GX,GY);                    \
}

    }

}


#endif


