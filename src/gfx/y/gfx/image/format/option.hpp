
//! \file

#ifndef Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED
#define Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED 1

#include "y/string.hpp"
#include "y/gfx/entity.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            class option : public entity
            {
            public:
                const string name;
                string       value;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(option);
            };

        }

    }

}

#endif
