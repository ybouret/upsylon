//! \file

#ifndef Y_GFX_OPS_GRAD_INCLUDED
#define Y_GFX_OPS_GRAD_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/gfx/color/histogram.hpp"
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
            typedef point2d<int8_t> vertex; //!< alias

           

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
            void compute(broker &apply, const pixmap<float> &); //!< compute -> gmax
            void maxima(broker  &apply, histogram           &); //!< keep only maxima
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            pixmap<vertex>   gdir; //!< digitized gradient direction
            pixmap<float>    edge; //!< edge working
            shared_filters   comp; //!< get components
            const float      gmax; //!< last max gradient

        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradient);
            const pixmap<float>   *host;
            void        compute(const tile &t) throw();
            static void compute(const tile &,void *,lockable&) throw();

            void        keepmax(const tile &t) throw();
            static void keepmax(const tile &,void *,lockable&) throw();

        };
        
    }
    
}

#endif
