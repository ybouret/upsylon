

#ifndef Y_JIVE_LANG_OPTION_INCLUDED
#define Y_JIVE_LANG_OPTION_INCLUDED 1

#include "y/jive/language/axiom/wildcard.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Option : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('O', 'P', 'T', 'N');

                template <typename ID> inline
                explicit Option(const ID &i, const Axiom &a) : Wildcard(i,UUID,a) {}
                virtual ~Option() throw();



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
            };

        }
    }
}

#endif
