//! \file

#ifndef Y_GFX_OPS_GRAD_INCLUDED
#define Y_GFX_OPS_GRAD_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/gfx/color/type-to-rgba.hpp"
#include "y/gfx/filters.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! computing the gradient
        //
        //______________________________________________________________________
        class gradient : public pixmap<float>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef point2d<float> vertex; //!< alias

           

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit gradient(const unit_t W, const unit_t H, const shared_filters &F); //!< setup
            virtual ~gradient() throw();                       //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void compute(const pixmap<float> &, broker &apply); //!< compute -> gmax
            void normalize(broker &apply) throw();              //!< normalize with gmax

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            pixmap<vertex>   g;    //!< map of normalized vertices
            shared_filters   comp; //!< get components
            const float      gmax; //!< last max gradient

        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradient);
            void compute(const pixmap<float> &, const tile &t) throw();
        };
        
    }
    
}

#endif
