
//! \file

#ifndef Y_MKL_ADJUST_COMMON_INCLUDED
#define Y_MKL_ADJUST_COMMON_INCLUDED 1

#include "y/mkl/fit/variables.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            //! common part of generic fit functions
            class Common
            {
            public:
                const Variable::Names names; //!< default names

                virtual      ~Common() throw();                  //!< cleanup
                void          appendTo( Variables &vars ) const; //!< append global variables
                static string Suffix(const size_t value);        //!< format a suffix string

            protected:
                explicit      Common(const size_t);              //!< setup with memory reservation

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Common);
            };

        }

    }

}

#endif

