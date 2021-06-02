

#ifndef Y_JIVE_LANG_COMPOUND_INCLUDED
#define Y_JIVE_LANG_COMPOUND_INCLUDED 1

#include "y/jive/language/axiom.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Compound : public Axiom, public Axiom::Manifest
            {
            public:

                virtual ~Compound() throw();

            protected:
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

        }

    }

}

#endif
