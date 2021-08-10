//! \file

#ifndef Y_GFX_ENHANCE_INCLUDED
#define Y_GFX_ENHANCE_INCLUDED 1

#include "y/gfx/pixmap.hpp"

namespace upsylon
{
    namespace graphic
    {
        //! scalar enhancing
        class enhance
        {
        public:
            explicit enhance() throw();
            virtual ~enhance() throw();

            template <typename T> inline
            void scalar(pixmap<T> & ,
                        broker    & )
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(enhance);
            float vmin;
            float vmax;
            float scal;
            
        };
    }
}

#endif

