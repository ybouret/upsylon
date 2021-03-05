
//! \file

#ifndef Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED
#define Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED 1

#include "y/string.hpp"
#include "y/gfx/entity.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            class option : public entity
            {
            public:
                typedef intr_ptr<string,option> pointer;

                const string name;
                string       value;

                template <typename NAME, typename VALUE>
                inline explicit option(const NAME &n, const VALUE &v) :
                name(n), value(v)
                {
                }

                virtual      ~option() throw();
                const string &key() const throw();

                friend std::ostream & operator<<(std::ostream &, const option&);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(option);
            };

        }

    }

}

#endif
