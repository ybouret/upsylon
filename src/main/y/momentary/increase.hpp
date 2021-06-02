//! \file

#ifndef Y_MOMENTARY_INCREASE_INCLUDED
#define Y_MOMENTARY_INCREASE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! make a temporary value
    //
    //__________________________________________________________________________
    template <typename T>
    class momentary_increase
    {
    public:

        //! ref=source,sav=source
        inline explicit momentary_increase(T      &variable,
                                           T       increase) :
        ref(variable),
        sav(ref)
        {
            variable += increase;
        }

        //! ref=sav
        inline virtual ~momentary_increase() throw()
        {
            ref=sav;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(momentary_increase);
        T &ref;
        T  sav;
    };

    //__________________________________________________________________________
    //
    //
    //! make a temporary value with a fixed value
    //
    //__________________________________________________________________________
    template <const int INCR, typename T>
    class momentary_increase_by : public momentary_increase<T>
    {
    public:
        //! variable += INCR
        inline explicit momentary_increase_by(T      &variable) :
        momentary_increase<T>(variable, T(INCR) )
        {
        }

        //! restore variable
        inline virtual ~momentary_increase_by() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(momentary_increase_by);
    };
}

#endif
