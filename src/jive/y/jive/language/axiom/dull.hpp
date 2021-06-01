
//! \file

#ifndef Y_JIVE_LANG_DULL_INCLUDED
#define Y_JIVE_LANG_DULL_INCLUDED 1

#include "y/jive/language/axiom/internal.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Dull : public Internal
            {
            public:
                static const uint32_t UUID   = Y_FOURCC('D','U','L','L');

                template <typename ID> inline
                explicit Dull(const ID &i) : Internal(i,UUID) {}
                virtual ~Dull() throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Dull);
            };
        }
    }
}

#endif
