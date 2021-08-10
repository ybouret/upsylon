//! \file

#ifndef Y_GFX_ENHANCE_INCLUDED
#define Y_GFX_ENHANCE_INCLUDED 1

#include "y/gfx/ops/extrema.hpp"

namespace upsylon
{
    namespace graphic
    {
        //! scalar enhancing
        class enhance
        {
        public:
            explicit enhance() throw(); //!< setup
            virtual ~enhance() throw(); //!< cleanup

            //! apply algo
            template <typename T> inline
            void scalar(pixmap<T> &source,
                        broker    &apply )
            {
                const T *arr = extrema::minmax(source,apply);
                vmin         = static_cast<float>(arr[0]);
                vmax         = static_cast<float>(arr[1]);
                vamp         = vmax-vmin;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(enhance);
            float vmin;
            float vmax;
            float vamp;
            
        };
    }
}

#endif

