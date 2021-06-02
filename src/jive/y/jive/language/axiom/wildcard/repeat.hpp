
#ifndef Y_JIVE_LANG_REPEAT_INCLUDED
#define Y_JIVE_LANG_REPEAT_INCLUDED 1

#include "y/jive/language/axiom/wildcard.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Repeat : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('R', 'E', 'P', 'T');
                
                template <typename ID> inline
                explicit Repeat(const ID &i, const Axiom &a) : Wildcard(i,UUID,a) {}
                virtual ~Repeat() throw();



                const size_t atLeast;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
            };

        }
    }
}

#endif
