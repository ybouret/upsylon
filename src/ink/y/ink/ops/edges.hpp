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
            inline void compute(const Blur       *blur,
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
                compute_with(E);
            }


            PixmapF pixels; //!< original pixels
            PixmapF grad_x; //!< grad_x
            PixmapF grad_y; //!< grad_y
            PixmapF grad;   //!< |grad|
            float   gmax;   //!< max|grad|
            PixmapF angle;  //!< angle of gradient
            PixmapF border; //!< local mininum

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            Engine::Function compute_metrics_func;
            Engine::Function analyze_borders_func;

            void             compute_metrics_call( const Tile &area, lockable &);
            void             analyze_borders_call( const Tile &area, lockable &);
            void             compute_with(Engine &E);
        };

    }
}

#endif

