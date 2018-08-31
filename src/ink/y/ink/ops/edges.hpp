//! \file
#ifndef Y_INK_EDGES_INCLUDED
#define Y_INK_EDGES_INCLUDED 1

#include "y/ink/ops/blur.hpp"
#include "y/ink/ops/filter.hpp"

namespace upsylon
{
    namespace Ink
    {

        class Edges : public Object
        {
        public:
            explicit Edges(const size_t W,
                           const size_t H);
            virtual ~Edges() throw();

            template <typename T>
            inline void compute_metrics(const Blur       *blur,
                                        const matrix<T> &dx,
                                        const matrix<T> &dy,
                                        Engine           &E)
            {
                //! local blur, using grad_x as temporary
                if( blur )
                {
                    blur->apply(pixels,grad_x,pixels,E);
                }
                // local gradient computation
                Filter::Stencil(grad_x, pixels, dx, E);
                Filter::Stencil(grad_y, pixels, dy, E);
                // and internal computation
                E.run(compute_metrics_func);
            }


            PixmapF pixels;
            PixmapF grad_x;
            PixmapF grad_y;
            PixmapF grad;
            PixmapF angle;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            Engine::Function compute_metrics_func;
            void             compute_metrics_call( const Area &area, lockable &);
        };

    }
}

#endif

