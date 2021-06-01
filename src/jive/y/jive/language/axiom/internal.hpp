//! \file

#ifndef Y_JIVE_LANG_INTERNAL_INCLUDED
#define Y_JIVE_LANG_INTERNAL_INCLUDED 1

#include "y/jive/language/axiom.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! Internal Axiom base class
            //
            //__________________________________________________________________
            class Internal : public Axiom
            {
            public:
                virtual ~Internal() throw(); //!< cleanup


            protected:
                //! setup
                template <typename ID>
                explicit Internal(const ID &i, const uint32_t t) :
                Axiom(i,t)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }

}

#endif
