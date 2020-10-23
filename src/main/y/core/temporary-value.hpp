
//! \file

#ifndef Y_CORE_TEMPORARY_VALUE_INCLUDED
#define Y_CORE_TEMPORARY_VALUE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core
    {
        //______________________________________________________________________
        //
        //! make a temporary value
        //______________________________________________________________________
        template <typename T>
        class temporary_value
        {
        public:

            //! ref=source,sav=source
            inline explicit temporary_value(T      &variable,
                                            T       replaced) :
            ref(variable),
            sav(ref)
            {
                variable = replaced;
            }

            //! ref=sav
            inline ~temporary_value() throw()
            {
                ref=sav;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_value);
            T &ref;
            T  sav;
        };
    }
}

#endif

