
//! \file

#ifndef Y_GFX_EDGES_GRADIENT_INCLUDED
#define Y_GFX_EDGES_GRADIENT_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/filters.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            //__________________________________________________________________
            //
            //
            //! compute gradient norm + digitized direction
            //
            //__________________________________________________________________
            class gradient : public pixmap<float>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef point2d<int8_t> vertex; //!< local direction type
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit gradient(const unit_t W, const unit_t H); //!< setup
                virtual ~gradient() throw();                       //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
               
                //! compute norm and digitized direction
                float compute(const pixmap<float>  &field,
                              broker               &apply,
                              const shared_filters &delta);
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                pixmap<vertex> probe; //!< pixmap of local directions

            private:
                Y_DISABLE_COPY_AND_ASSIGN(gradient);
            };
        }
    }

}

#endif
