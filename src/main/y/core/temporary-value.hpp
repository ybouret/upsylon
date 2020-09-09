
//! \file

#ifndef Y_CORE_TEMPORARY_VALUE_INCLUDED
#define Y_CORE_TEMPORARY_VALUE_INCLUDED 1

#include "y/type/args.hpp"

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
            Y_DECL_ARGS(T,type); //!< aliasses

            //! ref=source,sav=source
            inline explicit temporary_value(type      &variable,
                                            const_type replaced) :
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
            type      &ref;
            const_type sav;
        };
    }
}

#endif

