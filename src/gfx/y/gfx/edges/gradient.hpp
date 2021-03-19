
//! \file

#ifndef Y_GFX_EDGES_GRADIENT_INCLUDED
#define Y_GFX_EDGES_GRADIENT_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/filters.hpp"
#include "y/gfx/edges/dvertex.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            //__________________________________________________________________
            //
            //
            //! compute gradient norm + digitized directions
            //
            //__________________________________________________________________
            class gradient : public pixmap<float>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef dvertex::type vertex; //!< local direction

             

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
