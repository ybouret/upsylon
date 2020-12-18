
//! \file

#ifndef Y_MOMENTARY_VALUE_INCLUDED
#define Y_MOMENTARY_VALUE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! make a temporary value
    //
    //__________________________________________________________________________
    template <typename T>
    class momentary_value
    {
    public:

        //! ref=source,sav=source
        inline explicit momentary_value(T      &variable,
                                        T       replaced) :
        ref(variable),
        sav(ref)
        {
            variable = replaced;
        }

        //! ref=sav
        inline ~momentary_value() throw()
        {
            ref=sav;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(momentary_value);
        T &ref;
        T  sav;
    };
}

#endif

